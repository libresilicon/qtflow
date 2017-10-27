#ifndef PROJECTSELECTOR_H
#define PROJECTSELECTOR_H

#include <QDockWidget>
#include <QMenu>

#include "projectstreemodel.h"

#include "ui_projects.h"

#include "project.h"

class ProjectSelector : public QDockWidget
{
	Q_OBJECT

public:
	ProjectSelector(QWidget *parent);

	void setProject(Project*);

signals:
	void openFile(QString);

public slots:
	void refresh();
	void onContextMenu(const QPoint&);
	void onOpen(const QModelIndex&);

private:
	Ui::Projects *ui;

	Project *project;

	ProjectsTreeModel *projectMembers;
	QMenu *context;
};

#endif // PROJECTSELECTOR_H
