#ifndef PROJECTSELECTOR_H
#define PROJECTSELECTOR_H

#include <QDockWidget>
#include <QMenu>

#include "projectstreemodel.h"

#include "ui_projects.h"

class ProjectSelector : public QDockWidget
{
	Q_OBJECT

public:
	ProjectSelector(QWidget *parent);

	void setRootDir(QString path);

	void refresh();

signals:
	void openFile(QString);

public slots:
	void onContextMenu(const QPoint&);
	void onOpen(const QModelIndex&);

private:
	Ui::Projects *ui;

	QString rootdir;

	ProjectsTreeModel *projects;
	QMenu *context;
};

#endif // PROJECTSELECTOR_H
