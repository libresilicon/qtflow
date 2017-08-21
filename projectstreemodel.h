#ifndef PROJECTSTREEMODEL_H
#define PROJECTSTREEMODEL_H

#include <QAbstractItemModel>
#include <QFileSystemWatcher>
#include <QFileInfo>

class ProjectsItem
{
public:
	explicit ProjectsItem(const QVector<QVariant> &data, ProjectsItem *parentItem = 0);
	~ProjectsItem();

	void appendChild(ProjectsItem *child);

	bool setFileData(const QFileInfo &file);
	QFileInfo fileData() const;

	ProjectsItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	bool insertChildren(int position, int count, int columns);
	int row() const;
	ProjectsItem *parentItem();
	bool removeChildren(int position, int count);
	bool removeColumns(int position, int columns);
	int childNumber() const;
	bool setData(int column, const QVariant &value);

private:
	QList<ProjectsItem*> childItems;
	QVector<QVariant> itemData;
	QFileInfo fileInfoData;
	ProjectsItem *parent;
};

class ProjectsTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit ProjectsTreeModel(QObject *parent = 0);
	~ProjectsTreeModel();

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
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
	void onChange(const QString&);

private:
	ProjectsItem *rootItem;
	ProjectsItem *sourceItem;

	QString top;

	QFileSystemWatcher *watcher;
};

#endif // PROJECTSTREEMODEL_H
