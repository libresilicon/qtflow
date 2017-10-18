#include "synthesisoptions.h"

SynthesisOptions::SynthesisOptions(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SynthesisOptions),
	project(NULL)
{
	ui->setupUi(this);
}

void SynthesisOptions::on_buttonBox_accepted()
{
	if(project) {
		project->setSettingABC(ui->checkBoxRunABC->isChecked());
	}
	close();
}

void SynthesisOptions::on_buttonBox_rejected()
{
	close();
}

void SynthesisOptions::setProject(Project *p)
{
	project = p;
	if(project) {
		ui->checkBoxRunABC->setChecked(project->hasSettingABC());
	}
}
