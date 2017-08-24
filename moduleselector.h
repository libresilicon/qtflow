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
	void setTopLevel(QString);

public slots:
	void on_setTopModule_clicked();
	void on_closeButton_clicked();
	void onSetTopModule(bool);
	void onContextMenu(const QPoint&);

private:
	Ui::Modules *ui;

	ModulesListModel *moduleList;
	ModulesTreeModel *moduleTree;

	QMenu *context;
	QString sourcedir;
};

#endif // MODULESELECTOR_H
