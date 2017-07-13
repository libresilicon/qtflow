#include "modulestreemodel.h"

ModulesTreeModel::ModulesTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant ModulesTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
}

QModelIndex ModulesTreeModel::index(int row, int column, const QModelIndex &parent) const
{
}

QModelIndex ModulesTreeModel::parent(const QModelIndex &index) const
{
}

int ModulesTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

}

int ModulesTreeModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

}

QVariant ModulesTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    return QVariant();
}
