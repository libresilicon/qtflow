#include "projectsettings.h"

ProjectSettings::ProjectSettings(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ProjectSettings)
{
	ui->setupUi(this);
}
