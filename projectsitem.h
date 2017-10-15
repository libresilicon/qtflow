#ifndef PROJECTSITEM_H
#define PROJECTSITEM_H

#include <QFileInfo>
#include <QVector>
#include <QVariant>

class ProjectsItem
{
public:
	explicit ProjectsItem(QString name, ProjectsItem *parentItem = 0);
	~ProjectsItem();

	void appendChild(ProjectsItem *child);

	void setFileData(const QFileInfo &file);
	QFileInfo fileData() const;

	ProjectsItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	ProjectsItem *parentItem();
	bool removeChildren(int position, int count);

private:
	QFileInfo m_fileInfoData;

	ProjectsItem* m_parentItem;
	QVector<QVariant> m_itemData;
	QList<ProjectsItem*> m_childItems;
};

#endif // PROJECTSITEM_H
