#include "moduleselector.h"

#include <QTextStream>
#include <QMessageBox>

ModuleSelector::ModuleSelector(QWidget *parent):
	ui(new Ui::Modules),
	QDockWidget(parent),
	moduleTree(new ModulesTreeModel)
{
	moduleList = NULL;
	ui->setupUi(this);
	modulesContext = new QMenu(ui->listView);
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

void ModuleSelector::on_setTopModule_clicked()
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

void ModuleSelector::on_closeButton_clicked()
{
	hide();
}
