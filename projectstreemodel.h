#ifndef PROJECTSTREEMODEL_H
#define PROJECTSTREEMODEL_H

#include <QAbstractItemModel>
#include <QFileSystemWatcher>
#include <QFileInfo>

#include "project.h"

#include "projectsitem.h"

class ProjectsTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit ProjectsTreeModel(QObject *parent = 0);
	~ProjectsTreeModel();

	void setProject(Project *p);

	bool topModule(const QModelIndex &index) const;

	QString filePath(const QModelIndex &index);

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;

	bool removeRows(int position, int rows, const QModelIndex &parent);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:
	void onChange(const QString&);

private:
	void addSearchPath(const QString &path);
	void addFileNodes(ProjectsItem *folderItem);
	void addFolderNodes(const QString &path, ProjectsItem *top);

	ProjectsItem *rootItem;

	QFileSystemWatcher *watcher;

	Project *project;
};

#endif // PROJECTSTREEMODEL_H
