#include "projectstreemodel.h"

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
    rootItem->insertChildren(rootItem->childCount(), 1, rootItem->columnCount());
    sourceItem = rootItem->child(rootItem->childCount() - 1);
    sourceItem->setData(0, "Sources");
}

ProjectsTreeModel::~ProjectsTreeModel()
{
    delete rootItem;
    delete watcher;
}

void ProjectsTreeModel::addDirectory(const QString &path)
{
    if (!QDir(path).exists())
        return;

    QStack<QFileInfo> roots;
    QMap<QString, QVector<QFileInfo>> children;
    QDirIterator it(path, QStringList() << "*.v", QDir::Files, QDirIterator::Subdirectories);
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
        sourceItem->insertChildren(sourceItem->childCount(), 1, sourceItem->columnCount());
        ProjectsItem *current = sourceItem->child(sourceItem->childCount() - 1);
        current->setData(0, file.fileName());
        current->setFileData(file);
        if (children.contains(file.baseName()))
        {
            foreach (const QFileInfo &child, children[file.baseName()])
            {
                current->insertChildren(current->childCount(), 1, current->columnCount());
                current->child(current->childCount() - 1)->setData(0, child.fileName());
                current->child(current->childCount() - 1)->setFileData(child);
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

QVariant ProjectsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

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

