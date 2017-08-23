#include "options.h"

Options::Options(QDialog *parent) :
	QDialog(parent),
	ui(new Ui::Options)
{
	ui->setupUi(this);
}
