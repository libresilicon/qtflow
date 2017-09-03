#include "vcdtreeitem.h"
#include "vcdtreemodel.h"

#include <QStringList>

VcdTreeModel::VcdTreeModel(const QString &data, QObject *parent)
	: QAbstractItemModel(parent)
{
	QList<QVariant> rootData;
	rootData << "Title" << "Summary";
	rootItem = new VcdTreeItem(rootData);
	setupModelData(data.split(QString("\n")), rootItem);
}

VcdTreeModel::~VcdTreeModel()
{
	delete rootItem;
}

int VcdTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<VcdTreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant VcdTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	VcdTreeItem *item = static_cast<VcdTreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags VcdTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant VcdTreeModel::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex VcdTreeModel::index(int row, int column, const QModelIndex &parent)
			const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	VcdTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<VcdTreeItem*>(parent.internalPointer());

	VcdTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex VcdTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	VcdTreeItem *childItem = static_cast<VcdTreeItem*>(index.internalPointer());
	VcdTreeItem *parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int VcdTreeModel::rowCount(const QModelIndex &parent) const
{
	VcdTreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<VcdTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

void VcdTreeModel::setupModelData(const QStringList &lines, VcdTreeItem *parent)
{
	QList<VcdTreeItem*> parents;
	QList<int> indentations;
	parents << parent;
	indentations << 0;

	int number = 0;

	while (number < lines.count()) {
		int position = 0;
		while (position < lines[number].length()) {
			if (lines[number].at(position) != ' ')
				break;
			position++;
		}

		QString lineData = lines[number].mid(position).trimmed();

		if (!lineData.isEmpty()) {
			// Read the column data from the rest of the line.
			QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
			QList<QVariant> columnData;
			for (int column = 0; column < columnStrings.count(); ++column)
				columnData << columnStrings[column];

			if (position > indentations.last()) {
				// The last child of the current parent is now the new parent
				// unless the current parent has no children.

				if (parents.last()->childCount() > 0) {
					parents << parents.last()->child(parents.last()->childCount()-1);
					indentations << position;
				}
			} else {
				while (position < indentations.last() && parents.count() > 0) {
					parents.pop_back();
					indentations.pop_back();
				}
			}

			// Append a new item to the current parent's list of children.
			parents.last()->appendChild(new VcdTreeItem(columnData, parents.last()));
		}

		++number;
	}
}
