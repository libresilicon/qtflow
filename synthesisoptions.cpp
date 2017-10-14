#include "synthesisoptions.h"

SynthesisOptions::SynthesisOptions(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SynthesisOptions)
{
	ui->setupUi(this);
}
