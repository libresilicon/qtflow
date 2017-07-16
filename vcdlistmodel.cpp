#include "vcdlistmodel.h"

#include <QMimeData>

VcdListModel::VcdListModel(QObject *parent)
    : QAbstractListModel(parent)
    , _signals(QList<QPair<int, QString>>())
{
}

VcdListModel::~VcdListModel()
{
}

QList<int> VcdListModel::getSignals()
{
    QList<int> result;
    foreach (const auto &s, _signals)
        result << s.first;
    return result;
}

int VcdListModel::rowCount(const QModelIndex&) const
{
    return _signals.size();
}

QVariant VcdListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _signals.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return _signals.at(index.row()).second;

    return QVariant();
}

Qt::ItemFlags VcdListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    return Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}

bool VcdListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        QPair<int, QString> var;
        var.first = 0;
        var.second = value.toString();
        _signals.replace(index.row(), var);
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

bool VcdListModel::setEnum(const QModelIndex &index, int num)
{
    if (!index.isValid() || index.row() >= _signals.size())
        return false;

    _signals[index.row()].first = num;
    emit signalsChanged();
    return true;
}

bool VcdListModel::insertRows(int position, int rows, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
        _signals.insert(position, QPair<int, QString>());
    endInsertRows();
    return true;
}

QStringList VcdListModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

bool VcdListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("application/vnd.text.list"))
        return false;

    if (column > 0)
        return false;

    int beginRow;

    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = parent.row();
    else
        beginRow = rowCount(QModelIndex());

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;

    while (!stream.atEnd())
    {
        QString text;
        stream >> text;
        newItems << text;
        ++rows;
    }

    insertRows(beginRow, rows, QModelIndex());
    foreach (QString text, newItems)
    {
        QStringList csv = text.split(',');
        QModelIndex idx = index(beginRow, 0, QModelIndex());
        setData(idx, csv[1], Qt::EditRole);
        setEnum(idx, csv[0].toInt());
        beginRow++;
    }

    return true;
}
