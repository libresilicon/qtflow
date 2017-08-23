#include "settings.h"

#include <QTextStream>

Settings::Settings(QWidget *parent, QSettings *s) :
	ui(new Ui::Settings),
	QDialog(parent),
	settings(s)
{
	ui->setupUi(this);
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
