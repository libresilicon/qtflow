#include "vcdsignalviewtreeitem.h"
#include "vcdsignalviewtreemodel.h"

#include <QStringList>
#include <QTextStream>

VcdSignalViewTreeModel::VcdSignalViewTreeModel(vcd::VcdData data, QVector<QString> filter, QObject *parent) :
	QAbstractItemModel(parent),
	vcdData(data)
{
	rootData << "Signals";
	rootItem = new VcdSignalViewTreeItem(rootData);

	showSignals(filter);
}

VcdSignalViewTreeModel::~VcdSignalViewTreeModel()
{
	delete rootItem;
}

void VcdSignalViewTreeModel::showSignals(QVector<QString> filter)
{
	if(rootItem) delete rootItem;
	rootItem = new VcdSignalViewTreeItem(rootData);

	foreach(vcd::Var var, vcdData.vars()) {
		QString s;
		for(int i=0; i < var.hierarchical_name().size(); i++) {
			s += '/';
			s += QString::fromStdString(var.hierarchical_name().at(i));
		}
		if(filter.contains(s))
			rootItem->appendChild(var);
	}
}

int VcdSignalViewTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<VcdSignalViewTreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant VcdSignalViewTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	VcdSignalViewTreeItem *item = static_cast<VcdSignalViewTreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags VcdSignalViewTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant VcdSignalViewTreeModel::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex VcdSignalViewTreeModel::index(int row, int column, const QModelIndex &parent)
			const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	VcdSignalViewTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<VcdSignalViewTreeItem*>(parent.internalPointer());

	VcdSignalViewTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex VcdSignalViewTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	VcdSignalViewTreeItem *childItem = static_cast<VcdSignalViewTreeItem*>(index.internalPointer());
	VcdSignalViewTreeItem *parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int VcdSignalViewTreeModel::rowCount(const QModelIndex &parent) const
{
	VcdSignalViewTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<VcdSignalViewTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}
