#ifndef VCDLISTMODEL_H
#define VCDLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QPair>
#include <QString>

class VcdListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit VcdListModel(QObject *parent = 0);
    ~VcdListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool setEnum(const QModelIndex &index, int num);
    bool insertRows(int row, int count, const QModelIndex &parent) override;

    QStringList mimeTypes() const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

private:
    QList<QPair<int, QString>> _signals;
};

#endif // VCDLISTMODEL_H
