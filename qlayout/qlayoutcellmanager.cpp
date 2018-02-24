#include "qlayoutcellmanager.h"

QLayoutCellManager::QLayoutCellManager(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CellManager)
{
	ui->setupUi(this);
}

void QLayoutCellManager::addCell(QString instance, QString macro)
{
	m_macroTable[instance]=macro;
	refreshTable();
}

void QLayoutCellManager::on_treeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
	if(column==2) {
		if(item->checkState(column)==Qt::CheckState::Checked) {
			emit(setLEF(item->text(0),true));
		} else if(item->checkState(column)==Qt::CheckState::Unchecked) {
			emit(setLEF(item->text(0),false));
		}
	}
	if(column==3) {
		if(item->checkState(column)==Qt::CheckState::Checked) {
			emit(setGDS(item->text(0),true));
		} else if(item->checkState(column)==Qt::CheckState::Unchecked) {
			emit(setGDS(item->text(0),false));
		}
	}
}

void QLayoutCellManager::refreshTable()
{
	QStringList header;
	QTreeWidgetItem* m;
	QList<QTreeWidgetItem*> items;
	foreach(QString k, m_macroTable.keys()) {
		m = new QTreeWidgetItem();
		m->setText(0,k);
		m->setText(1,m_macroTable[k]);
		m->setCheckState(2, Qt::Unchecked); // 2 == LEF
		m->setCheckState(3, Qt::Unchecked); // 3 == GDS
		items.append(m);
	}
	ui->treeWidget->clear();
	header << "Instance" << "Macro" << "Display LEF" << "Display GDS";
	ui->treeWidget->setHeaderLabels(header);
	ui->treeWidget->setColumnCount(header.count());
	ui->treeWidget->insertTopLevelItems(0, items);
}
