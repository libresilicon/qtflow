#include "testbenchtoolbox.h"

TestBenchToolBox::TestBenchToolBox(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::TestBenchToolBox)
{
	ui->setupUi(this);
	setMaximumHeight(ui->dockWidgetContents->height()+20);
	connect(ui->toolButtonStart,SIGNAL(clicked(bool)),this,SLOT(onRunSimulation()));
}

void TestBenchToolBox::onRunSimulation()
{
	emit(runSimulation());
}
