#include "moduleselector.h"

#include <QTextStream>
#include <QMessageBox>

ModuleSelector::ModuleSelector(QWidget *parent):
	ui(new Ui::Modules),
	QDockWidget(parent),
	moduleTree(new ModulesTreeModel)
{
	ui->setupUi(this);

	moduleList = NULL;

	context = new QMenu(ui->listView);
	QAction *setModulesTopModule = new QAction("Set as Top Module", context);
	context->addAction(setModulesTopModule);

	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));

	connect(setModulesTopModule, SIGNAL(triggered(bool)), this, SLOT(setTopModule_clicked()));
}

void ModuleSelector::onContextMenu(const QPoint &point)
{
	QModelIndex index = ui->listView->indexAt(point);

	if (!index.isValid())
		return;

	//setModulesTopModule->setDisabled(modules->topModule(index));
	context->exec(ui->listView->mapToGlobal(point));
}

void ModuleSelector::setSourceDir(QString path)
{
	sourcedir = path;
	refresh();
}

void ModuleSelector::refresh()
{
	if (sourcedir == QString())
		return;

	if(moduleList) delete moduleList;
	moduleList = new ModulesListModel(this,sourcedir);
	ui->listView->setModel(moduleList);
}

void ModuleSelector::onSetTopModule(bool)
{
	QString index = moduleList->data(ui->listView->currentIndex()).toString();
	//projects->setTopModule(index);
	//modules->setTopModule(index);
	moduleTree->setTopModule(index);
	emit(setTopLevel(index));
}

void ModuleSelector::setTopModule_clicked()
{
	QString top = moduleList->data(ui->listView->currentIndex()).toString();
	QString origin = moduleList->file(ui->listView->currentIndex());
	QFile target(sourcedir + "/" + top + ".v");
	QFile file(sourcedir + "/" + origin + ".v");
	if (top != origin)
	{
		QMessageBox dialog;
		dialog.setText("Rename " + origin + ".v to " + top + ".v?");
		dialog.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
		dialog.setDefaultButton(QMessageBox::No);
		int code = dialog.exec();
		switch (code)
		{
			case QMessageBox::Yes:
				if(!file.rename(target.fileName()))
					return;
				break;
			default:
				return;
		}
	}

	emit topModuleChanged();
	refresh();
}

void ModuleSelector::closeButton_clicked()
{
	hide();
}
