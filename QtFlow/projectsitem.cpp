#include "projectsitem.h"

ProjectsItem::ProjectsItem(QString name, ProjectsItem *parentItem)
{
	QVector<QVariant> rootData;
	rootData << name;

	m_parentItem = parentItem;
	m_itemData = rootData;
}

ProjectsItem::~ProjectsItem()
{
	qDeleteAll(m_childItems);
}

void ProjectsItem::appendChild(ProjectsItem *child)
{
	m_childItems.append(child);
}

void ProjectsItem::setFileData(const QFileInfo &file)
{
	m_fileInfoData = file;
}

QFileInfo ProjectsItem::fileData() const
{
	return m_fileInfoData;
}

ProjectsItem *ProjectsItem::child(int row)
{
	return m_childItems.value(row);
}

int ProjectsItem::childCount() const
{
	return m_childItems.count();
}

//childNumber
int ProjectsItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<ProjectsItem*>(this));

	return 0;
}

int ProjectsItem::columnCount() const
{
	return m_itemData.count();
}

QVariant ProjectsItem::data(int column) const
{
	return m_itemData.value(column);
}

ProjectsItem *ProjectsItem::parentItem()
{
	return m_parentItem;
}

bool ProjectsItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > m_childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
		delete m_childItems.takeAt(position);

	return true;
}
