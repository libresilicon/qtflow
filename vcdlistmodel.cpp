#include "vcdlistmodel.h"

#include <QDebug>
#include <QMimeData>

VcdListModel::VcdListModel(QObject *parent)
    : QAbstractListModel(parent)
    , _signals(QList<QPair<int, QString>>())
{
}

VcdListModel::~VcdListModel()
{
}

void VcdListModel::reset()
{
    _signals.clear();
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

    if (role == Qt::TextAlignmentRole)
        return QVariant(Qt::AlignVCenter | Qt::AlignRight);

    if (role == Qt::DisplayRole)
        return _signals.at(index.row()).second;

    return QVariant();
}

Qt::ItemFlags VcdListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    return Qt::ItemIsDragEnabled | QAbstractItemModel::flags(index);
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
    return true;
}

int VcdListModel::getEnum(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return _signals.at(index.row()).first;
}

bool VcdListModel::insertRows(int position, int rows, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
        _signals.insert(position, QPair<int, QString>());
    endInsertRows();
    return true;
}

bool VcdListModel::removeRows(int position, int rows, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
        _signals.removeAt(position);
    endRemoveRows();
    return true;
}

bool VcdListModel::moveRow(const QModelIndex&, int sourceRow, const QModelIndex&, int destinationChild)
{
    int length = _signals.count();

    if (destinationChild >= length)
        insertRows(length - 1, length - destinationChild + 1, QModelIndex());

    _signals.move(sourceRow, destinationChild);

    if (destinationChild >= length)
        removeRows(sourceRow + 1, 1, QModelIndex());

    return true;
}

QStringList VcdListModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData *VcdListModel::mimeData(const QModelIndexList &is) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, is)
    {
        if (index.isValid())
        {
            QString num = QString::number(getEnum(index));
            QString ele = QString::number(index.row());
            QString text(num + "," + ele + "," + data(index, Qt::DisplayRole).toString());
            stream << text;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
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

    foreach (QString text, newItems)
    {
        QStringList csv = text.split(',');
        if (csv.count() < 3)
            break;

        int origin = csv[1].toInt();
        if (csv[1] != QString() && origin > -1)
        {
            moveRow(QModelIndex(), origin, QModelIndex(), beginRow);
            beginRow++;
            break;
        }

        insertRows(beginRow, 1, QModelIndex());
        QModelIndex idx = index(beginRow, 0, QModelIndex());
        setData(idx, csv[2], Qt::EditRole);
        setEnum(idx, csv[0].toInt());
        beginRow++;
    }

    emit signalsChanged();
    return true;
}
