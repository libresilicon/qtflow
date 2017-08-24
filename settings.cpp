#include "settings.h"

#include <QTextStream>

Settings::Settings(QWidget *parent, QSettings *s) :
	QDialog(parent),
	ui(new Ui::Settings),
	settings(s)
{
	ui->setupUi(this);
	connect(ui->yosysButton,SIGNAL(clicked(bool)),this,SLOT(on_selectYosys_triggered()));
	connect(ui->graywolfButton,SIGNAL(clicked(bool)),this,SLOT(on_selectGraywolf_triggered()));
	connect(ui->qrouterButton,SIGNAL(clicked(bool)),this,SLOT(on_selectQRouter_triggered()));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(on_buttonBox_save()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(on_buttonBox_close()));

	// loading config
	ui->pathYosys->setText(settings->value("yosys").toString());
	ui->pathGraywolf->setText(settings->value("graywolf").toString());
	ui->pathQRouter->setText(settings->value("qrouter").toString());
}

void Settings::on_selectYosys_triggered()
{
	QTextStream(stdout) << "Miau\n";
}

void Settings::on_selectGraywolf_triggered()
{
	QTextStream(stdout) << "Miau\n";
}

void Settings::on_selectQRouter_triggered()
{
	QTextStream(stdout) << "Miau\n";
}

void Settings::on_buttonBox_save()
{
	settings->value("yosys",ui->pathYosys->displayText());
	settings->sync();
	close();
}

void Settings::on_buttonBox_close()
{
	close();
}
