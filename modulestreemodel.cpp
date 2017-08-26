#include "modulestreemodel.h"

#include "constants.h"

#include <QFont>
#include <QFile>
#include <QStack>
#include <QSet>
#include <QDirIterator>
#include <QTextStream>

ModulesItem::ModulesItem(const QVector<QVariant> &data, ModulesItem *parentItem)
{
	parent = parentItem;
	itemData = data;
}

ModulesItem::~ModulesItem()
{
	qDeleteAll(childItems);
}

void ModulesItem::appendChild(ModulesItem *child)
{
	childItems.append(child);
}

bool ModulesItem::setFileData(const QFileInfo &file)
{
	fileInfoData = file;
	return true;
}

QFileInfo ModulesItem::fileData() const
{
	return fileInfoData;
}

ModulesItem *ModulesItem::child(int row)
{
	return childItems.value(row);
}

int ModulesItem::childCount() const
{
	return childItems.count();
}

int ModulesItem::row() const
{
	if (parent)
		return parent->childItems.indexOf(const_cast<ModulesItem*>(this));

	return 0;
}

int ModulesItem::columnCount() const
{
	return itemData.count();
}

QVariant ModulesItem::data(int column) const
{
	return itemData.value(column);
}

bool ModulesItem::setData(int column, const QVariant &value)
{
	if (column < 0 || column >= itemData.size())
		return false;

	itemData[column] = value;
	return true;
}

ModulesItem *ModulesItem::parentItem()
{
	return parent;
}

bool ModulesItem::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
	{
		QVector<QVariant> data(columns);
		ModulesItem *item = new ModulesItem(data, this);
		childItems.insert(position, item);
	}

	return true;
}

bool ModulesItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
		delete childItems.takeAt(position);

	return true;
}


ModulesTreeModel::ModulesTreeModel(QObject *parent)
	: QAbstractItemModel(parent)
	, watcher(new QFileSystemWatcher(parent))
{
	QVector<QVariant> rootData;
	rootData << "Item";
	rootItem = new ModulesItem(rootData);
}

ModulesTreeModel::~ModulesTreeModel()
{
	delete rootItem;
	delete watcher;
}

void ModulesTreeModel::reset()
{
	beginResetModel();
	removeRows(0, rootItem->childCount(), createIndex(0, 0, rootItem));
	QVector<QVariant> rootData;
	rootData << "Item";
	rootItem = new ModulesItem(rootData);
	endResetModel();
}

void ModulesTreeModel::setRootPath(const QString &path)
{
	if (!QDir(path).exists())
		return;

	delete watcher;
	watcher = new QFileSystemWatcher(this);
	watcher->addPath(path);
	connect(watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(onChange(const QString&)));
	connect(watcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(onChange(const QString&)));

	reset();

	QRegExp rx("^\\s*module ([a-zA-Z0-9_]+)");
	QDirIterator it(path, QStringList() << "*.v", QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext())
	{
		QFileInfo info(it.next());
		QFile file(info.absoluteFilePath());

		if (info.completeSuffix() == info.suffix() && file.open(QIODevice::ReadOnly))
		{
			watcher->addPath(info.absoluteFilePath());

			QTextStream in(&file);
			while (!in.atEnd())
			{
				QString line = in.readLine();
				if (rx.indexIn(line, 0) != -1)
				{
					QString cap(rx.cap(1));
					rootItem->insertChildren(rootItem->childCount(), 1, rootItem->columnCount());
					ModulesItem *current = rootItem->child(rootItem->childCount() - 1);
					current->setData(0, cap);
					current->setFileData(file);
				}
			}

			file.close();
		}
	}
}

QString ModulesTreeModel::filePath(const QModelIndex &index)
{
	if (!index.isValid())
		return QString();

	ModulesItem *item = static_cast<ModulesItem*>(index.internalPointer());
	return item->fileData().absoluteFilePath();
}

QVariant ModulesTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex ModulesTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	ModulesItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ModulesItem*>(parent.internalPointer());

	ModulesItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();

}

QModelIndex ModulesTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	ModulesItem *childItem = static_cast<ModulesItem*>(index.internalPointer());
	ModulesItem *parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}


bool ModulesTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	ModulesItem *parentItem;
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ModulesItem*>(parent.internalPointer());

	bool success = true;
	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int ModulesTreeModel::rowCount(const QModelIndex &parent) const
{
	ModulesItem *parentItem;
	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<ModulesItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int ModulesTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<ModulesItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

bool ModulesTreeModel::topModule(const QModelIndex &index) const
{
	return data(index, Qt::DisplayRole) == QVariant(top);
}

void ModulesTreeModel::setTopModule(QString t)
{
	top = t;
}

void ModulesTreeModel::setTestBench(QString t)
{
	testbench = t;
}

QVariant ModulesTreeModel::data(const QModelIndex &index, int role) const
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

	ModulesItem *item = static_cast<ModulesItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags ModulesTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

void ModulesTreeModel::onChange(const QString&)
{
//	Session &s = Session::Instance();
//	setRootPath(s.project());
}
