#include "projectsettings.h"

#include <QTextStream>

ProjectSettings::ProjectSettings(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ProjectSettings)
{
	ui->setupUi(this);
	QDomNodeList nl;

	settingsFileProcess = new QDomDocument();
	QFile file( ":configs/process.xml" );
	if(file.open(QIODevice::ReadOnly)) {
		settingsFileProcess->setContent(&file);
		file.close();
	}

	nl = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl.count(); i++) {
		QDomElement e = nl.at(i).toElement();
		ui->comboBoxTechnology->addItem(e.attribute("xml:id"));
	}

	connect(ui->comboBoxTechnology,SIGNAL(activated(int)), this, SLOT(technologyActivated(int)));
	connect(ui->comboBoxProcess,SIGNAL(activated(int)), this, SLOT(processActivated(int)));

	technologyActivated(0);
}

void ProjectSettings::technologyActivated(int i)
{
	QString currentTech = ui->comboBoxTechnology->currentText();
	ui->comboBoxProcess->clear();

	QDomNodeList nl = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl.count(); i++) {
		QDomElement e = nl.at(i).toElement();
		if(currentTech==e.attribute("xml:id")) {
			QDomNodeList cn = e.childNodes();
			for(int j = 0; j< cn.count(); j++) {
				QDomElement pe = cn.at(j).toElement();
				if(pe.tagName()=="title") {
					ui->techTitle->setText(pe.text());
				}
				if(pe.tagName()=="description") {
					ui->techDescription->setText(pe.text());
				}
				if(pe.tagName()=="process") {
					ui->comboBoxProcess->addItem(pe.attribute("name"));
				}
			}
		}
	}
	processActivated(i);
}

void ProjectSettings::processActivated(int i)
{
	QString currentProcess = ui->comboBoxProcess->currentText();
	QDomNodeList nl = settingsFileProcess->elementsByTagName("process");
	for(i = 0; i< nl.count(); i++) {
		QDomElement e = nl.at(i).toElement();
		if(currentProcess==e.attribute("name")) {
			ui->processDescription->setText(e.attribute("description"));
		}
	}
}
