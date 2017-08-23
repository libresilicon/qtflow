#include "modules.h"
#include "ui_modules.h"

#include "constants.h"

#include <QDirIterator>
#include <QDebug>
#include <QListView>
#include <QMessageBox>

Modules::Modules(QWidget *parent, QSettings* settings) :
	QDialog(parent),
	ui(new Ui::Modules),
	path(QString()),
	modules(new ModulesListModel(parent,settings))
{
	ui->setupUi(this);
}

Modules::~Modules()
{
	delete ui;
}

void Modules::refresh()
{
	if (path == QString())
		return;
	refresh(path);
}

void Modules::setPath(QString dir)
{
	path = dir;
}

void Modules::refresh(QString dir)
{
	path = dir;
	QSettings *s = new QSettings(dir);
	if(modules) delete modules;
	modules = new ModulesListModel(this, s);
	ui->listView->setModel(modules);
}

void Modules::on_setTopModule_clicked()
{
	QString top = modules->data(ui->listView->currentIndex()).toString();
	QTextStream(stdout) << "New top module: " << top << '\n';
	QString origin = modules->file(ui->listView->currentIndex());
	QFile target(path + "/" + top + ".v");
	QFile file(path + "/" + origin + ".v");
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

void Modules::on_closeButton_clicked()
{
	hide();
}
