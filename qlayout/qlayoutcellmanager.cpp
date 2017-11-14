#include "qlayoutcellmanager.h"

QLayoutCellManager::QLayoutCellManager(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CellManager)
{
	ui->setupUi(this);
}
