#include "schematicsparametricpartdialog.h"

SchematicsParametricPartDialog::SchematicsParametricPartDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ParametricPartDialog)
{
	ui->setupUi(this);
}
