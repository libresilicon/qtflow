#include "vcdtreeitem.h"
#include "vcdsignaltreemodel.h"

#include <QStringList>

VcdSignalTreeModel::VcdSignalTreeModel(const vcd::VcdData &data, QObject *parent)
	: QAbstractItemModel(parent)
{
	QList<QVariant> rootData;
	rootData << "Signals";
	rootItem = new VcdTreeItem(rootData);
	vcdData = data;
}

VcdSignalTreeModel::~VcdSignalTreeModel()
{
	delete rootItem;
}

void VcdSignalTreeModel::showSignals(QStringList list)
{
	foreach(vcd::Var var, vcdData.vars()) {
		std::string s;
		for(int i=0; i < var.hierarchical_name().size(); i++) {
			s = var.hierarchical_name().at(i);
			rootItem->appendChild(QString::fromStdString(s));
		}
	}
}

int VcdSignalTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<VcdTreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant VcdSignalTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	VcdTreeItem *item = static_cast<VcdTreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags VcdSignalTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant VcdSignalTreeModel::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex VcdSignalTreeModel::index(int row, int column, const QModelIndex &parent)
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

QModelIndex VcdSignalTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	VcdTreeItem *childItem = static_cast<VcdTreeItem*>(index.internalPointer());
	VcdTreeItem *parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int VcdSignalTreeModel::rowCount(const QModelIndex &parent) const
{
	VcdTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<VcdTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}
