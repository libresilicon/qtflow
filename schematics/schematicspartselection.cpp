#include "schematicspartselection.h"

SchematicsPartSelection::SchematicsPartSelection(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PartSelection),
	lefdata(NULL)
{
	ui->setupUi(this);
	hide();
}

void SchematicsPartSelection::on_buttonBox_accepted()
{
	hide();
}

void SchematicsPartSelection::on_buttonBox_rejected()
{
	hide();
}

void SchematicsPartSelection::setLEF(lef::LEFData *d)
{
	lefdata = d;
	updatePartList();
}

void SchematicsPartSelection::updatePartList()
{
	if(lefdata) {
		ui->partList->clear();
		foreach(lef::LEFMacro *m, lefdata->getMacros()) {
			ui->partList->addItem(m->getName());
		}
	}
}

void SchematicsPartSelection::on_partList_currentTextChanged(QString s)
{
	if(lefdata) {

	}
	qDebug() << "Current changed " << s;
}
