#ifndef QLAYOUTCELLMANAGER_H
#define QLAYOUTCELLMANAGER_H

#include <QDialog>

#include "ui_cellmanager.h"

class QLayoutCellManager : public QDialog
{
public:
	QLayoutCellManager(QWidget *parent = Q_NULLPTR);
private:
	Ui::CellManager* ui;
};

#endif // QLAYOUTCELLMANAGER_H
