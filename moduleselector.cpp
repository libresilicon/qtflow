#include "moduleselector.h"

#include <QTextStream>
#include <QMessageBox>

ModuleSelector::ModuleSelector(QWidget *parent):
	QDockWidget(parent),
	ui(new Ui::Modules),
	moduleTree(new ModulesTreeModel),
	project(NULL),
	moduleList(NULL)
{
	QAction *setModulesTestbench;
	QAction *setModulesTopModule;

	ui->setupUi(this);
	ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);

	context = new QMenu(ui->listView);

	setModulesTopModule = new QAction("Set as Top Module", context);
	context->addAction(setModulesTopModule);
	connect(setModulesTopModule, SIGNAL(triggered(bool)), this, SLOT(onSetTopModule(bool)));

	setModulesTestbench = new QAction("Set as Top Testbench", context);
	context->addAction(setModulesTestbench);
	connect(setModulesTestbench, SIGNAL(triggered(bool)), this, SLOT(onSetTestBench(bool)));

	connect(ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
}

void ModuleSelector::onContextMenu(const QPoint &point)
{
	QModelIndex index = ui->listView->indexAt(point);

	if (!index.isValid())
		return;

	context->exec(ui->listView->mapToGlobal(point));
}

void ModuleSelector::setProject(Project *p)
{
	project = p;
	if(project) {
		sourcedir = project->getSourceDir();
		toplevel = project->getTopLevel();
		testbench = project->getTestBench();
	}
	refresh();
}

void ModuleSelector::refresh()
{
	if (sourcedir == QString())
		return;

	if(moduleList) delete moduleList;
	moduleList = new ModulesListModel(this,sourcedir);
	moduleList->setTopLevel(toplevel);
	moduleList->setTestBench(testbench);
	ui->listView->setModel(moduleList);
}

void ModuleSelector::onSetTopModule(bool)
{
	QString index = moduleList->data(ui->listView->currentIndex()).toString();
	moduleTree->setTopModule(index);
	toplevel = index;
	emit(setTopLevel(index));
	refresh();
}

void ModuleSelector::onSetTestBench(bool)
{
	QString index = moduleList->data(ui->listView->currentIndex()).toString();
	moduleTree->setTestBench(index);
	testbench = index;
	emit(setTestBench(index));
	refresh();
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
