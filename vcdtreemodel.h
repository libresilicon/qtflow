#ifndef VCDTREEMODEL_H
#define VCDTREEMODEL_H

#include <QAbstractItemModel>
#include "vcdcommon.h"

class VcdItem
{
public:
    explicit VcdItem(const QVector<QVariant> &data, VcdItem *parentItem = 0);
    ~VcdItem();

    void appendChild(VcdItem *child);

    VcdItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    int row() const;
    VcdItem *parentItem();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);
    bool setEnum(int num);
    int getEnum() const;

private:
    QList<VcdItem*> childItems;
    QVector<QVariant> itemData;
    int itemEnum;
    VcdItem *parent;
};

class VcdTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit VcdTreeModel(QObject *parent = 0);
    ~VcdTreeModel();

    void reset();

    void setVcd(vcd_t);
    vcd_t& vcd();

    vcd_changes_t getValues(int);

    void setScopes(vcd_scopes_t scopes, VcdItem *item);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    bool removeRows(int position, int rows, const QModelIndex &parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int getEnum(const QModelIndex &index) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList &is) const override;

private:
    VcdItem *rootItem;

    vcd_t values;
};

#endif // VCDTREEMODEL_H
