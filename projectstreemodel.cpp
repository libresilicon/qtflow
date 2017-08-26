#include "projectstreemodel.h"

#include "constants.h"

#include <QFont>
#include <QFile>
#include <QStack>
#include <QSet>
#include <QDirIterator>

ProjectsItem::ProjectsItem(const QVector<QVariant> &data, ProjectsItem *parentItem)
{
	parent = parentItem;
	itemData = data;
}

ProjectsItem::~ProjectsItem()
{
	qDeleteAll(childItems);
}

void ProjectsItem::appendChild(ProjectsItem *child)
{
	childItems.append(child);
}

bool ProjectsItem::setFileData(const QFileInfo &file)
{
	fileInfoData = file;
	return true;
}

QFileInfo ProjectsItem::fileData() const
{
	return fileInfoData;
}

ProjectsItem *ProjectsItem::child(int row)
{
	return childItems.value(row);
}

int ProjectsItem::childCount() const
{
	return childItems.count();
}

//childNumber
int ProjectsItem::row() const
{
	if (parent)
		return parent->childItems.indexOf(const_cast<ProjectsItem*>(this));

	return 0;
}

int ProjectsItem::columnCount() const
{
	return itemData.count();
}

QVariant ProjectsItem::data(int column) const
{
	return itemData.value(column);
}

bool ProjectsItem::setData(int column, const QVariant &value)
{
	if (column < 0 || column >= itemData.size())
		return false;

	itemData[column] = value;
	return true;
}

ProjectsItem *ProjectsItem::parentItem()
{
	return parent;
}

bool ProjectsItem::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
	{
		QVector<QVariant> data(columns);
		ProjectsItem *item = new ProjectsItem(data, this);
		childItems.insert(position, item);
	}

	return true;
}

bool ProjectsItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
		delete childItems.takeAt(position);

	return true;
}


ProjectsTreeModel::ProjectsTreeModel(QObject *parent)
	: QAbstractItemModel(parent)
	, watcher(new QFileSystemWatcher(parent))
{
	QVector<QVariant> rootData;
	rootData << "Item";
	rootItem = new ProjectsItem(rootData);
	rootItem->insertChildren(rootItem->childCount(), 3, rootItem->columnCount());
	sourceItem = rootItem->child(rootItem->childCount() - 1);
	sourceItem->setData(0, "Sources Bar");
	layoutItem = rootItem->child(rootItem->childCount() - 1);
	layoutItem->setData(0, "Layouts");
	schematicsItem = rootItem->child(rootItem->childCount() - 1);
	schematicsItem->setData(0, "Schematics");
}

ProjectsTreeModel::~ProjectsTreeModel()
{
	delete rootItem;
	delete watcher;
}

void ProjectsTreeModel::reset()
{
	beginResetModel();
	removeRows(0, rootItem->childCount(), createIndex(0, 0, rootItem));
	rootItem->insertChildren(0, 3, rootItem->columnCount());
	sourceItem = rootItem->child(0);
	sourceItem->setData(0, "Sources");
	layoutItem = rootItem->child(1);
	layoutItem->setData(0, "Layouts");
	schematicsItem = rootItem->child(2);
	schematicsItem->setData(0, "Schematics");
	endResetModel();
}

void ProjectsTreeModel::setRootPath(const QString &path)
{
	if (!QDir(path).exists())
		return;

	delete watcher;
	watcher = new QFileSystemWatcher(this);
	watcher->addPath(path);
	connect(watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(onChange(const QString&)));
	connect(watcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(onChange(const QString&)));

	reset();

	QStack<QFileInfo> roots;
	QMap<QString, QVector<QFileInfo>> children;
	QDirIterator it(path, QStringList() << "*.v" << "*.mag" << "*.sch" << "*.sym", QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext())
	{
		QFileInfo file(it.next());
		watcher->addPath(file.absoluteFilePath());

		if (file.completeSuffix() == file.suffix())
		{
			roots.push(file);
		}
		else
		{
			QVector<QFileInfo> infos;
			if (!children.contains(file.baseName()))
			{
				infos << file;
				children.insert(file.baseName(), infos);
			}
			else
			{
				children[file.baseName()] << file;
			}
		}
	}

	while (!roots.isEmpty())
	{
		QFileInfo file = roots.pop();
		if(file.suffix()=="mag") {
			layoutItem->insertChildren(layoutItem->childCount(), 1, layoutItem->columnCount());
			ProjectsItem *current = layoutItem->child(layoutItem->childCount() - 1);
			current->setData(0, file.fileName());
			current->setFileData(file);
		} else if(file.suffix()=="sch"||file.suffix()=="sym") {
			schematicsItem->insertChildren(schematicsItem->childCount(), 1, schematicsItem->columnCount());
			ProjectsItem *current = schematicsItem->child(schematicsItem->childCount() - 1);
			current->setData(0, file.fileName());
			current->setFileData(file);
		} else if(file.suffix()=="v"||file.suffix()=="hs") {
			sourceItem->insertChildren(sourceItem->childCount(), 1, sourceItem->columnCount());
			ProjectsItem *current = sourceItem->child(sourceItem->childCount() - 1);
			current->setData(0, file.fileName());
			current->setFileData(file);

			if (children.contains(file.baseName()))
			{
				foreach (const QFileInfo &child, children[file.baseName()])
				{
					current->insertChildren(current->childCount(), 1, current->columnCount());
					ProjectsItem *sub_current = current->child(current->childCount() - 1);
					sub_current->setData(0, child.fileName());
					sub_current->setFileData(child);
				}
			}
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
	return data(index, Qt::DisplayRole) == QVariant(top + ".v");
}

void ProjectsTreeModel::setTopModule(QString t)
{
	top = t;
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
