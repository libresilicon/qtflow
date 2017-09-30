#include "layoutvisibles.h"

LayoutVisibles::LayoutVisibles(QWidget *parent):
	QDockWidget(parent),
	ui(new Ui::LayoutVisibles),
	project(NULL)
{
	ui->setupUi(this);
	ui->typesTree->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->searchText->setEnabled(true);

	connect(ui->typesTree,SIGNAL(clicked(QModelIndex)),this,SLOT(handleClick(QModelIndex)));
	connect(ui->searchText,SIGNAL(textChanged(QString)),this,SLOT(handleSearch(QString)));
}

void LayoutVisibles::handleClick(const QModelIndex &index)
{
	emit(refreshLayout());
}

void LayoutVisibles::handleSearch(QString s)
{
	QString name;
	QTreeWidgetItem *m;
	foreach(m, typeEntries){
		name=m->data(2,0).toString();
		if(name.contains(s))
			m->setHidden(false);
		else
			m->setHidden(true);
	}
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
		ui->typesTree->clear();
		typeEntries.clear();

		foreach(QString layern, project->getPlanes()) {
			treeitem = new QTreeWidgetItem(ui->typesTree);
			treeitem->setFlags(treeitem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
			treeitem->setCheckState(0,typeIsEnabled(layern)?Qt::Checked:Qt::Unchecked);
			treeitem->setIcon(1,project->materialIcon(layern));
			treeitem->setText(2,layern);
			typeEntries.append(treeitem);

			foreach(QString vname, project->getType(layern)) {
				if(vname==layern)
					continue;

				treeChileItem = new QTreeWidgetItem(treeitem);
				treeChileItem->setFlags(treeChileItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
				treeChileItem->setCheckState(0,typeIsEnabled(vname)?Qt::Checked:Qt::Unchecked);
				treeChileItem->setIcon(1,project->materialIcon(vname));
				treeChileItem->setText(2,vname);
				typeEntries.append(treeChileItem);
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
			qDebug() << m->checkState(0);
			return (m->checkState(0)==Qt::CheckState::Checked);
		}
	}
	qDebug() << "Undefined: " << s;
	return true;
}
