#include "buildflow.h"

BuildFlow::BuildFlow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::BuildFlow),
	project(NULL)
{
	ui->setupUi(this);
	connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(on_buttonBox_accepted()));
}

void BuildFlow::setProject(Project* p)
{
	project = p;
	if(project) displayBuildSteps();
}

void BuildFlow::displayBuildSteps()
{
}

void BuildFlow::on_buttonBox_accepted()
{
	close();
}

void BuildFlow::on_buttonBox_rejected()
{
	close();
}
