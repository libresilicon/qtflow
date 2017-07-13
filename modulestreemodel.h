#ifndef MODULESTREEMODEL_H
#define MODULESTREEMODEL_H

#include <QAbstractItemModel>

class ModulesTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ModulesTreeModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // MODULESTREEMODEL_H
