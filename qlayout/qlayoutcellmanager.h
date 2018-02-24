#ifndef QLAYOUTCELLMANAGER_H
#define QLAYOUTCELLMANAGER_H

#include <QDialog>

#include "ui_cellmanager.h"

class QLayoutCellManager : public QDialog
{
	Q_OBJECT
public:
	QLayoutCellManager(QWidget *parent = Q_NULLPTR);
	void addCell(QString instance, QString macro);

public slots:
	void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);

signals:
	void setGDS(QString,bool);
	void setLEF(QString,bool);

private:
	void refreshTable();
	Ui::CellManager* ui;
	QMap<QString,QString> m_macroTable;
};

#endif // QLAYOUTCELLMANAGER_H
