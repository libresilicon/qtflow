#ifndef MODULESELECTOR_H
#define MODULESELECTOR_H

#include "ui_modules.h"
#include "moduleslistmodel.h"
#include "modulestreemodel.h"

#include "project.h"

#include <QDockWidget>
#include <QSettings>
#include <QFile>
#include <QMenu>

class ModuleSelector : public QDockWidget
{
	Q_OBJECT

public:
	ModuleSelector(QWidget *parent);
	void setProject(Project *project);

signals:
	void topModuleChanged();
	void setTopLevel(QString);
	void setTestBench(QString);

public slots:
	void setTopModule_clicked();
	void closeButton_clicked();
	void onSetTopModule(bool);
	void onSetTestBench(bool);
	void onContextMenu(const QPoint&);
	void refresh();

private:
	Ui::Modules *ui;

	Project *project;

	ModulesListModel *moduleList;
	ModulesTreeModel *moduleTree;

	QMenu *context;
	QString sourcedir;

	QString testbench;
	QString toplevel;
};

#endif // MODULESELECTOR_H
