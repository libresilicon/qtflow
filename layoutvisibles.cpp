#include "layoutvisibles.h"

LayoutVisibles::LayoutVisibles(QWidget *parent):
	QDockWidget(parent),
	ui(new Ui::LayoutVisibles),
	project(NULL)
{
	ui->setupUi(this);
	ui->layerList->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->layerList,SIGNAL(clicked(QModelIndex)),this,SLOT(handleClick(QModelIndex)));
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
	QListWidgetItem* item;
	QPixmap pm(100,100);
	if(project) {
		foreach(QString layern, project->getLayers()) {
			item = new QListWidgetItem(ui->layerList);
			item->setText(layern);
			pm.fill(project->colorMat(layern));
			item->setIcon(QIcon(pm));
			item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
			item->setCheckState(Qt::Checked);
		}
		foreach(QString layern, project->getVisibles()) {
			item = new QListWidgetItem(ui->renderList);
			item->setText(layern);
			pm.fill(project->colorMat(layern));
			item->setIcon(QIcon(pm));
			item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
			item->setCheckState(Qt::Checked);
		}
	}
}

void LayoutVisibles::on_layerList_customContextMenuRequested(const QPoint &pos)
{
	QPoint globalPos;
	QMenu menu;

	globalPos = ui->layerList->mapToGlobal(pos);
	menu.addAction("Change color");
	menu.exec(globalPos);
}

void LayoutVisibles::changeColor()
{

}

bool LayoutVisibles::layerIsEnabled(QString s)
{
	bool ret = true;
	QListWidgetItem *m;
	for(int i = 0; i < ui->layerList->count(); ++i){
		m = ui->layerList->item(i);
		if(m) if(m->text()==s)
			ret = m->checkState();
	}
	return ret;
}

bool LayoutVisibles::visibleIsEnabled(QString)
{
	bool ret = true;
	QListWidgetItem *m;
	for(int i = 0; i < ui->renderList->count(); ++i){
		m = ui->layerList->item(i);
		qDebug() << m->text();
	}
	return ret;
}
