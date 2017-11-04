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
	int i;
	QStringList m_TableHeader;
	QStringList data;

	ui->powerTable->clear();
	m_TableHeader.clear();

	m_TableHeader<< "Name" << "Layer";
	ui->powerTable->setHorizontalHeaderLabels(m_TableHeader);
	ui->groundTable->setHorizontalHeaderLabels(m_TableHeader);
	ui->clockTable->setHorizontalHeaderLabels(m_TableHeader);

	ui->powerTable->setColumnCount(m_TableHeader.count());
	ui->groundTable->setColumnCount(m_TableHeader.count());
	ui->clockTable->setColumnCount(m_TableHeader.count());

	data = project->getPowerNets();
	ui->powerTable->setRowCount(data.count());
	for(int i=0; i<data.count(); i++) {
		ui->powerTable->setItem(i, 0, new QTableWidgetItem(data.at(i)));
		ui->powerTable->setItem(i, 1, new QTableWidgetItem(project->getSpecialNetLayer(data.at(i))));
	}

	data = project->getGroundNets();
	ui->groundTable->setRowCount(data.count());
	for(int i=0; i<data.count(); i++) {
		ui->groundTable->setItem(i, 0, new QTableWidgetItem(data.at(i)));
		ui->groundTable->setItem(i, 1, new QTableWidgetItem(project->getSpecialNetLayer(data.at(i))));
	}

	data = project->getClockNets();
	ui->clockTable->setRowCount(data.count());
	for(int i=0; i<data.count(); i++) {
		ui->clockTable->setItem(i, 0, new QTableWidgetItem(data.at(i)));
		ui->clockTable->setItem(i, 1, new QTableWidgetItem(project->getSpecialNetLayer(data.at(i))));
	}
}
