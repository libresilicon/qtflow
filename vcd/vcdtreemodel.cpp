#include "vcd/vcd_data.hpp"
#include "vcdtreeitem.h"
#include "vcdtreemodel.h"

#include <QStringList>

VcdTreeModel::VcdTreeModel(const vcd::VcdData &data, QObject *parent)
	: QAbstractItemModel(parent)
{
	QList<QVariant> rootData;
	rootData << "SST";
	rootItem = new VcdTreeItem(rootData);
	buildTree(data.vars());
}

VcdTreeModel::~VcdTreeModel()
{
	delete rootItem;
}

void VcdTreeModel::buildTree(std::vector<vcd::Var> data)
{
	VcdTreeItem *subItem = rootItem;
	foreach(vcd::Var var, data) {
		std::string s;
		for(int i=0; i+1 < var.hierarchical_name().size(); i++) {
			s = var.hierarchical_name().at(i);
			subItem = subItem->appendChild(QString::fromStdString(s));
		}
		subItem = rootItem;
	}
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

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<VcdTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}
