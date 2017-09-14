#ifndef MODULESTREEMODEL_H
#define MODULESTREEMODEL_H

#include <QAbstractItemModel>
#include <QFileSystemWatcher>
#include <QFileInfo>

class ModulesItem
{
public:
    explicit ModulesItem(const QVector<QVariant> &data, ModulesItem *parentItem = 0);
    ~ModulesItem();

    void appendChild(ModulesItem *child);

    bool setFileData(const QFileInfo &file);
    QFileInfo fileData() const;

    ModulesItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    int row() const;
    ModulesItem *parentItem();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:
    QList<ModulesItem*> childItems;
    QVector<QVariant> itemData;
    QFileInfo fileInfoData;
    ModulesItem *parent;
};

class ModulesTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ModulesTreeModel(QObject *parent = 0);
    ~ModulesTreeModel();

    void reset();

    void setRootPath(const QString &path);
    QString filePath(const QModelIndex &index);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    bool removeRows(int position, int rows, const QModelIndex &parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool topModule(const QModelIndex &index) const;
    void setTopModule(QString);
	void setTestBench(QString);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
    void onChange(const QString&);

private:
    ModulesItem *rootItem;
    ModulesItem *sourceItem;

    QString top;
	QString testbench;

    QFileSystemWatcher *watcher;
};

#endif // MODULESTREEMODEL_H
