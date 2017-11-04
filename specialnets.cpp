#include "specialnets.h"

SpecialNets::SpecialNets(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SpecialNets)
{
	ui->setupUi(this);
}

void SpecialNets::setProject(Project *p)
{
	project = p;
	refreshTables();
}

void SpecialNets::refreshTables()
{
	foreach(QString powernet,project->getPowerNets()) {

	}
}
