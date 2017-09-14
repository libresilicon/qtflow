#include "projectsettings.h"

#include <QTextStream>

ProjectSettings::ProjectSettings(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ProjectSettings),
	project(NULL)
{
	ui->setupUi(this);
	QDomNodeList nl;

	settingsFileProcess = new QDomDocument();
	QFile file(":/process.xml");
	if(file.open(QIODevice::ReadOnly)) {
		settingsFileProcess->setContent(&file);
		file.close();
	}

	QString tech;
	nl = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl.count(); i++) {
		QDomElement e = nl.at(i).toElement();
		tech = e.attribute("xml:id");
		ui->comboBoxTechnology->addItem(tech,tech);
	}

	connect(ui->comboBoxTechnology,SIGNAL(activated(int)), this, SLOT(technologyActivated(int)));
	connect(ui->comboBoxProcess,SIGNAL(activated(int)), this, SLOT(processActivated(int)));
	connect(ui->buttonBox,SIGNAL(accepted()), this, SLOT(storeData()));

	technologyActivated(0);
}

void ProjectSettings::technologyActivated(int i)
{
	QString currentTech = ui->comboBoxTechnology->currentText();
	ui->comboBoxProcess->clear();

	QString processName;
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
					processName = pe.attribute("xml:id");
					ui->comboBoxProcess->addItem(processName,processName);
				}
			}
		}
	}

	if(project) ui->comboBoxProcess->setCurrentIndex(ui->comboBoxProcess->findData(project->getProcess()));

	processActivated(i);
}

void ProjectSettings::processActivated(int i)
{
	QString currentProcess = ui->comboBoxProcess->currentText();
	QDomNodeList nl = settingsFileProcess->elementsByTagName("process");
	for(i = 0; i< nl.count(); i++) {
		QDomElement e = nl.at(i).toElement();
		if(currentProcess==e.attribute("xml:id")) {
			QDomNodeList cn = e.childNodes();
			for(int j = 0; j< cn.count(); j++) {
				QDomElement pe = cn.at(j).toElement();
				if(pe.tagName()=="description") {
					ui->processDescription->setText(pe.text());
				}
			}
		}
	}
}

void ProjectSettings::storeData()
{
	if(project) {
		project->setTechnology(ui->comboBoxTechnology->currentText());
		project->setProcess(ui->comboBoxProcess->currentText());
	}
}

void ProjectSettings::setProject(Project *p)
{
	project = p;
	if(project) {
		ui->comboBoxTechnology->setCurrentIndex(ui->comboBoxTechnology->findData(project->getTechnology()));
		technologyActivated(0);
	}
}
