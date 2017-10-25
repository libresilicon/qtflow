#include "layoutvisibles.h"

LayoutVisibles::LayoutVisibles(QWidget *parent):
	QDockWidget(parent),
	ui(new Ui::LayoutVisibles),
	project(NULL)
{
	ui->setupUi(this);
	ui->typesTree->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->searchText->setEnabled(true);

	connect(ui->typesTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(handleClick(QTreeWidgetItem*,int)));
	connect(ui->searchText,SIGNAL(textChanged(QString)),this,SLOT(handleSearch(QString)));
}

void LayoutVisibles::handleClick(QTreeWidgetItem *item, int column)
{
	QTreeWidgetItem *m;
	if(typeRootEntries.contains(item)) {
		foreach(m, typeEntries){
			if(m->parent()==item) {
				m->setCheckState(0,item->checkState(0));
			}
		}
	}

	emit(enabledTypesChanged(getEnabledTypes()));
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

void LayoutVisibles::onRegisterLayer(QString s)
{
	QTreeWidgetItem *m;
	QString planen;
	QString vname;

	foreach(planen, project->getPlanes()) {
		foreach(vname, project->getTypes(planen)) {
			if(vname==s) return;
		}
	}
	foreach(m, typeEntries){
		if(m->data(2,0).toString()==s) return;
	}

	qDebug() << "Registering layer " << s;

	m = new QTreeWidgetItem(ui->typesTree);
	m->setCheckState(0,typeIsEnabled(s)?Qt::Checked:Qt::Unchecked);
	if(project) m->setIcon(1,project->materialIcon(s));
	m->setText(2,s);
	m->setFlags(m->flags()|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
	typeEntries.append(m);
}

void LayoutVisibles::refreshLists()
{
	QString planen;
	QString vname;
	QTreeWidgetItem* treeitem;
	QTreeWidgetItem* treeChileItem;

	if(project) {
		ui->typesTree->clear();
		typeEntries.clear();
		typeRootEntries.clear();

		foreach(planen, project->getPlanes()) {

			treeitem = new QTreeWidgetItem(ui->typesTree);

			treeitem->setBackground(0, QColor(Qt::gray));
			treeitem->setBackground(1, QColor(Qt::gray));
			treeitem->setBackground(2, QColor(Qt::gray));

			treeitem->setCheckState(0,typeIsEnabled(planen)?Qt::Checked:Qt::Unchecked);
			treeitem->setIcon(1,project->materialIcon(planen));
			treeitem->setText(2,planen);
			treeitem->setFlags(treeitem->flags()|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);

			typeRootEntries.append(treeitem);

			foreach(vname, project->getTypes(planen)) {
				treeChileItem = new QTreeWidgetItem(treeitem);
				treeChileItem->setCheckState(0,typeIsEnabled(vname)?Qt::Checked:Qt::Unchecked);
				treeChileItem->setIcon(1,project->materialIcon(vname));
				treeChileItem->setText(2,vname);
				treeChileItem->setFlags(treeChileItem->flags()|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
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
			return (m->checkState(0)==Qt::CheckState::Checked);
		}
	}
	return true;
}

QStringList LayoutVisibles::getEnabledTypes()
{
	QTreeWidgetItem *m;
	QStringList ret;
	foreach(m, typeEntries){
		if(m->checkState(0)==Qt::CheckState::Checked)
				ret << m->data(2,0).toString();
	}
	return ret;
}
