#ifndef MODULESELECTOR_H
#define MODULESELECTOR_H

#include "ui_modules.h"
#include "moduleslistmodel.h"
#include "modulestreemodel.h"

#include <QDockWidget>
#include <QSettings>
#include <QFile>
#include <QMenu>

class ModuleSelector : public QDockWidget
{
	Q_OBJECT

public:
	ModuleSelector(QWidget *parent);

	void refresh();

	void setSourceDir(QString path);

signals:
	void topModuleChanged();

private slots:
	void on_setTopModule_clicked();
	void on_closeButton_clicked();

private:
	Ui::Modules *ui;

	ModulesListModel *moduleList;
	ModulesTreeModel *moduleTree;

	QMenu *modulesContext;
	QString sourcedir;
};

#endif // MODULESELECTOR_H
