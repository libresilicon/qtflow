#include "projectstreemodel.h"

#include <QFont>
#include <QFile>
#include <QStack>
#include <QSet>
#include <QDirIterator>

ProjectsTreeModel::ProjectsTreeModel(QObject *parent) :
	QAbstractItemModel(parent),
	watcher(new QFileSystemWatcher(parent)),
	project(NULL)
{
	rootItem = new ProjectsItem("Project member");
}

ProjectsTreeModel::~ProjectsTreeModel()
{
	delete rootItem;
	delete watcher;
}

void ProjectsTreeModel::setProject(Project *p)
{
	project = p;

	delete watcher;
	watcher = new QFileSystemWatcher(this);

	if(project) foreach(QString path, project->getSearchDirectories()) {
		addFolderNodes(path,rootItem);
	}
}

void ProjectsTreeModel::addFolderNodes(const QString &path, ProjectsItem *top)
{
	QDir dir(path);
	QStringList fileList;

	ProjectsItem *folderItem;

	folderItem = new ProjectsItem(QFileInfo(path).baseName(), top);
	folderItem->setFileData(QFileInfo(path));
	top->appendChild(folderItem);

	addFileNodes(folderItem);

	dir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );
	fileList = dir.entryList();
	for (int i=0; i<fileList.count(); i++) {
		addFolderNodes(dir.filePath(fileList[i]),folderItem);
	}
}

void ProjectsTreeModel::addFileNodes(ProjectsItem *folderItem)
{
	ProjectsItem *current;
	ProjectsItem *sourceItem = new ProjectsItem("Sources", folderItem);
	ProjectsItem *layoutItem = new ProjectsItem("Layouts", folderItem);
	ProjectsItem *schematicsItem = new ProjectsItem("Schematics", folderItem);
	ProjectsItem *othersItem = new ProjectsItem("Others", folderItem);

	QDir dir(folderItem->fileData().absoluteFilePath());

	folderItem->appendChild(sourceItem);
	folderItem->appendChild(layoutItem);
	folderItem->appendChild(schematicsItem);
	folderItem->appendChild(othersItem);

	dir.setFilter( QDir::Files | QDir::NoDotAndDotDot );
	QStringList fileList = dir.entryList();
	for (int i=0; i<fileList.count(); i++) {
		QFileInfo file(dir.filePath(fileList[i]));

		if(file.suffix()=="mag"||file.suffix()=="def") {
			current = new ProjectsItem(file.fileName(), layoutItem);
			current->setFileData(file);
			layoutItem->appendChild(current);
		} else if(file.suffix()=="sch"||file.suffix()=="sym") {
			current = new ProjectsItem(file.fileName(), schematicsItem);
			current->setFileData(file);
			schematicsItem->appendChild(current);
		} else if(file.suffix()=="v"||file.suffix()=="hs") {
			current = new ProjectsItem(file.fileName(), sourceItem);
			current->setFileData(file);
			sourceItem->appendChild(current);
		} else {
			current = new ProjectsItem(file.fileName(), othersItem);
			current->setFileData(file);
			othersItem->appendChild(current);
		}
	}
}

QString ProjectsTreeModel::filePath(const QModelIndex &index)
{
	if (!index.isValid())
		return QString();

	ProjectsItem *item = static_cast<ProjectsItem*>(index.internalPointer());
	return item->fileData().absoluteFilePath();
}

QVariant ProjectsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex ProjectsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	ProjectsItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ProjectsItem*>(parent.internalPointer());

	ProjectsItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();

}

QModelIndex ProjectsTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	ProjectsItem *childItem = static_cast<ProjectsItem*>(index.internalPointer());
	ProjectsItem *parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}


bool ProjectsTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	ProjectsItem *parentItem;
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ProjectsItem*>(parent.internalPointer());

	bool success = true;
	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int ProjectsTreeModel::rowCount(const QModelIndex &parent) const
{
	ProjectsItem *parentItem;
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ProjectsItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int ProjectsTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<ProjectsItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

bool ProjectsTreeModel::topModule(const QModelIndex &index) const
{
	if(project)
		return (data(index, Qt::DisplayRole) == QVariant(project->getTopLevelFile()));

	return false;
}

QVariant ProjectsTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::FontRole)
	{
		QFont font;
		if (!topModule(index))
			return font;
		font.setBold(true);
		return font;
	}

	if (role != Qt::DisplayRole)
		return QVariant();

	ProjectsItem *item = static_cast<ProjectsItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags ProjectsTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

void ProjectsTreeModel::onChange(const QString&)
{
}
