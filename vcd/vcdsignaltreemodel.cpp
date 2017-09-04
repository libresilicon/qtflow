#include "vcdsignaltreeitem.h"
#include "vcdsignaltreemodel.h"

#include <QStringList>
#include <QTextStream>

VcdSignalTreeModel::VcdSignalTreeModel(vcd::VcdData data, QVector<QString> filter, QObject *parent) :
	QAbstractItemModel(parent),
	vcdData(data)
{
	rootData << "Type";
	rootData << "Signals";
	rootItem = new VcdSignalTreeItem(rootData);

	showSignals(filter);
}

VcdSignalTreeModel::~VcdSignalTreeModel()
{
	delete rootItem;
}

void VcdSignalTreeModel::showSignals(QVector<QString> filter)
{
	if(rootItem) delete rootItem;
	rootItem = new VcdSignalTreeItem(rootData);

	bool ok;
	foreach(vcd::Var var, vcdData.vars()) {
		QString s;
		for(int i=0; i < var.hierarchical_name().size(); i++) {
			s = QString::fromStdString(var.hierarchical_name().at(i));
			if(filter.count()) {
				if(i<filter.count()) {
					ok=(filter.at(i)==s)&&(filter.count()+1==var.hierarchical_name().size());
				} else if(i==filter.count()) {
					if(ok) rootItem->appendChild(var);
				}
			}
		}
	}
}

int VcdSignalTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<VcdSignalTreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant VcdSignalTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	VcdSignalTreeItem *item = static_cast<VcdSignalTreeItem*>(index.internalPointer());

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

	VcdSignalTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<VcdSignalTreeItem*>(parent.internalPointer());

	VcdSignalTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex VcdSignalTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	VcdSignalTreeItem *childItem = static_cast<VcdSignalTreeItem*>(index.internalPointer());
	VcdSignalTreeItem *parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int VcdSignalTreeModel::rowCount(const QModelIndex &parent) const
{
	VcdSignalTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<VcdSignalTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}
