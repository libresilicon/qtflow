#include "vcdtreemodel.h"

#include <QFile>
#include <QStack>
#include <QSet>
#include <QDirIterator>
#include <QTextStream>

VcdItem::VcdItem(const QVector<QVariant> &data, VcdItem *parentItem)
{
    parent = parentItem;
    itemData = data;
}

VcdItem::~VcdItem()
{
    qDeleteAll(childItems);
}

void VcdItem::appendChild(VcdItem *child)
{
    childItems.append(child);
}

VcdItem *VcdItem::child(int row)
{
    return childItems.value(row);
}

int VcdItem::childCount() const
{
    return childItems.count();
}

int VcdItem::row() const
{
    if (parent)
        return parent->childItems.indexOf(const_cast<VcdItem*>(this));

    return 0;
}

int VcdItem::columnCount() const
{
    return itemData.count();
}

QVariant VcdItem::data(int column) const
{
    return itemData.value(column);
}

bool VcdItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

VcdItem *VcdItem::parentItem()
{
    return parent;
}

bool VcdItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        QVector<QVariant> data(columns);
        VcdItem *item = new VcdItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool VcdItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}


VcdTreeModel::VcdTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , values(vcd_t())
{
    QVector<QVariant> rootData;
    rootData << "Scopes";
    rootItem = new VcdItem(rootData);
}

VcdTreeModel::~VcdTreeModel()
{
    delete rootItem;
}

void VcdTreeModel::setVcd(vcd_t v)
{
    values = v;
    removeRows(0, rootItem->childCount(), createIndex(0, 0, rootItem));
    setScopes(values.scopes, rootItem);
}

void VcdTreeModel::setScopes(vcd_scopes_t scopes, VcdItem *item)
{
    item->insertChildren(item->childCount(), 1, item->columnCount());
    VcdItem *current = item->child(item->childCount() - 1);
    current->setData(0, scopes.name);

    QHash<int, QString>::const_iterator it;
    for (it = scopes.alias.constBegin(); it != scopes.alias.constEnd(); ++it)
    {
        current->insertChildren(current->childCount(), 1, current->columnCount());
        current->child(current->childCount() - 1)->setData(0, it.value());
    }

    QList<vcd_scopes_t>::const_iterator is;
    for (is = scopes.scopes.constBegin(); is != scopes.scopes.constEnd(); ++is)
        setScopes(*is, current);
}

vcd_t& VcdTreeModel::vcd()
{
    return values;
}

QVariant VcdTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex VcdTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    VcdItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<VcdItem*>(parent.internalPointer());

    VcdItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();

}

QModelIndex VcdTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    VcdItem *childItem = static_cast<VcdItem*>(index.internalPointer());
    VcdItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


bool VcdTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    VcdItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<VcdItem*>(parent.internalPointer());

    bool success = true;
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int VcdTreeModel::rowCount(const QModelIndex &parent) const
{
    VcdItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<VcdItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int VcdTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<VcdItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant VcdTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    VcdItem *item = static_cast<VcdItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags VcdTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
