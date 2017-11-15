#ifndef QLAYOUTCELLMANAGER_H
#define QLAYOUTCELLMANAGER_H

#include <QDialog>

#include "project.h"

#include "ui_cellmanager.h"

class QLayoutCellManager : public QDialog
{
public:
	QLayoutCellManager(QWidget *parent = Q_NULLPTR);
	void setProject(Project *p);
	void addCell(QString instance, QString macro);
private:
	void refreshTable();
	Ui::CellManager* ui;
	Project *project;
	QMap<QString,QString> m_macroTable;
};

#endif // QLAYOUTCELLMANAGER_H
