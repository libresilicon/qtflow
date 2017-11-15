#include "qlayoutcellmanager.h"

QLayoutCellManager::QLayoutCellManager(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CellManager),
	project(NULL)
{
	ui->setupUi(this);
}

void QLayoutCellManager::addCell(QString instance, QString macro)
{
	m_macroTable[instance]=macro;
}

void QLayoutCellManager::setProject(Project *p)
{
	project = p;
	if(project) {
		refreshTable();
	}
}

void QLayoutCellManager::refreshTable()
{

}
