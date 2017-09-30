#include "layoutvisibles.h"

LayoutVisibles::LayoutVisibles(QWidget *parent):
	QDockWidget(parent),
	ui(new Ui::LayoutVisibles),
	project(NULL)
{
	ui->setupUi(this);
	ui->typesTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->typesTree,SIGNAL(clicked(QModelIndex)),this,SLOT(handleClick(QModelIndex)));
}

void LayoutVisibles::handleClick(const QModelIndex &index)
{
	emit(refreshLayout());
}

void LayoutVisibles::setProject(Project *p)
{
	project = p;
	refreshLists();
}

void LayoutVisibles::refreshLists()
{
	QTreeWidgetItem* treeitem;
	QTreeWidgetItem* treeChileItem;
	if(project) {
		foreach(QString layern, project->getPlanes()) {
			QPixmap pm(100,100);
			treeitem = new QTreeWidgetItem(ui->typesTree);
			pm.fill(project->colorMat(layern));
			treeitem->setFlags(treeitem->flags() | Qt::ItemIsUserCheckable);
			treeitem->setCheckState(0,Qt::Checked);
			treeitem->setIcon(1,QIcon(pm));
			treeitem->setText(2,layern);
			typeEntries.append(treeitem);
			foreach(QString typeName, project->getTypeNames()) {
				foreach(QString vname, project->getType(typeName)) {
					QPixmap pm(100,100);
					treeChileItem = new QTreeWidgetItem(treeitem);
					pm.fill(project->colorMat(vname));
					treeChileItem->setFlags(treeChileItem->flags() | Qt::ItemIsUserCheckable);
					treeChileItem->setCheckState(0,Qt::Checked);
					treeChileItem->setIcon(1,QIcon(pm));
					treeChileItem->setText(2,vname);
					typeEntries.append(treeChileItem);
				}
			}
		}
	}
}

void LayoutVisibles::on_layerList_customContextMenuRequested(const QPoint &pos)
{
	QPoint globalPos;
	QMenu menu;

	globalPos = ui->typesTree->mapToGlobal(pos);
	menu.addAction("Change color");
	menu.exec(globalPos);
}

void LayoutVisibles::changeColor()
{

}

bool LayoutVisibles::typeIsEnabled(QString s)
{
	QTreeWidgetItem *m;
	foreach(m, typeEntries){
		if(m->data(2,0).toString()==s) {
			return (m->checkState(0)==Qt::CheckState::Checked);
		}
	}
	qDebug() << "Undefined: " << s;
	return true;
}
