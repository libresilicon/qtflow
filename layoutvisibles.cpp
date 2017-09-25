#include "layoutvisibles.h"

LayoutVisibles::LayoutVisibles(QWidget *parent):
	QDockWidget(parent),
	ui(new Ui::LayoutVisibles),
	project(NULL)
{
	ui->setupUi(this);
	ui->layerList->setContextMenuPolicy(Qt::CustomContextMenu);
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
	menu.addAction("This is a type 1");
	menu.exec(globalPos);
}

void LayoutVisibles::changeColor()
{

}
