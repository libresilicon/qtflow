#include "settings.h"

#include <QTextStream>
#include <QFileDialog>

Settings::Settings(QWidget *parent, QSettings *s) :
	QDialog(parent),
	ui(new Ui::Settings),
	settings(s)
{
	ui->setupUi(this);
	connect(ui->yosysButton,SIGNAL(clicked(bool)),this,SLOT(selectYosys_triggered()));
	connect(ui->graywolfButton,SIGNAL(clicked(bool)),this,SLOT(selectGraywolf_triggered()));
	connect(ui->qrouterButton,SIGNAL(clicked(bool)),this,SLOT(selectQRouter_triggered()));
	connect(ui->icarusButton,SIGNAL(clicked(bool)),this,SLOT(selectIcarus_triggered()));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(buttonBox_save()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(buttonBox_close()));

	// loading config
	ui->pathYosys->setText(settings->value("yosys").toString());
	ui->pathGraywolf->setText(settings->value("graywolf").toString());
	ui->pathQRouter->setText(settings->value("qrouter").toString());
	ui->pathIcarus->setText(settings->value("icarus").toString());
}

void Settings::selectYosys_triggered()
{
	ui->pathYosys->setText(QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath()));
}

void Settings::selectGraywolf_triggered()
{
	ui->pathGraywolf->setText(QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath()));
}

void Settings::selectQRouter_triggered()
{
	ui->pathQRouter->setText(QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath()));
}

void Settings::selectIcarus_triggered()
{
	ui->pathIcarus->setText(QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath()));
}

void Settings::buttonBox_save()
{
	settings->setValue("yosys",ui->pathYosys->displayText());
	settings->setValue("graywolf",ui->pathGraywolf->displayText());
	settings->setValue("qrouter",ui->pathQRouter->displayText());
	settings->setValue("icarus",ui->pathIcarus->displayText());

	emit(syncSettings());
	emit(buttonBox_close());
}

void Settings::buttonBox_close()
{
	close();
}
