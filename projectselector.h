#ifndef PROJECTSELECTOR_H
#define PROJECTSELECTOR_H

#include <QDockWidget>
#include "projectstreemodel.h"

#include "ui_projects.h"

class ProjectSelector : public QDockWidget
{
	Q_OBJECT

public:
	ProjectSelector(QWidget *parent);

	void setSourceDir(QString path);

	void refresh();

signals:

public slots:

private:
	Ui::Projects *ui;

	QString sourcedir;

	ProjectsTreeModel *projects;
};

#endif // PROJECTSELECTOR_H
