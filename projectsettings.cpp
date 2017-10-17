#include "projectsettings.h"

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

	connect(ui->comboBoxTechnology,SIGNAL(activated(int)), this, SLOT(technologyActivated()));
	connect(ui->comboBoxProcess,SIGNAL(activated(int)), this, SLOT(processActivated()));
	connect(ui->buttonBox,SIGNAL(accepted()), this, SLOT(storeData()));

	technologyActivated();

	setSearchPathButtons();
	setSearchPathList();
}

void ProjectSettings::setSearchPathList()
{
	if(project) {
		ui->searchPaths->clear();
		foreach(QString s, project->getSearchDirectories())
			ui->searchPaths->addItem(s);
	}
}

void ProjectSettings::setSearchPathButtons()
{
	QToolBar *toolbar;
	QAction *button;

	toolbar = new QToolBar(ui->groupBoxSearchPaths);
	ui->groupBoxSearchPaths->layout()->addWidget(toolbar);

	button = new QAction(QPixmap(":/add.svg"), "Add directory", toolbar);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(addDirectory()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/delete.svg"), "Delete directory", toolbar);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(deleteDirectory()));
	toolbar->addAction(button);
}

void ProjectSettings::addDirectory()
{
	QString d = QFileDialog::getExistingDirectory ( this, "Choose search path", project->getSourceDir() );
	if(d==QString()) return;
	ui->searchPaths->addItem(d);
}

void ProjectSettings::deleteDirectory()
{
	QListWidgetItem *m = ui->searchPaths->currentItem();
	QListWidgetItem *o;
	QModelIndex index = ui->searchPaths->currentIndex();
	int i;

	if(m) {
		i = index.row();
		o = ui->searchPaths->takeItem(i);
	}
}

void ProjectSettings::technologyActivated()
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

	processActivated();
}

void ProjectSettings::processActivated()
{
	QString currentProcess = ui->comboBoxProcess->currentText();
	QDomNodeList nl = settingsFileProcess->elementsByTagName("process");
	for(int i = 0; i< nl.count(); i++) {
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
	QListWidgetItem *item;
	QStringList paths;

	if(project) {
		project->setTechnology(ui->comboBoxTechnology->currentText());
		project->setProcess(ui->comboBoxProcess->currentText());
		project->setSimulationScript(ui->pathSimulationScript->text());
		project->setSynthesisScript(ui->pathSynthesisScript->text());

		for(int row = 0; row < ui->searchPaths->count(); row++)
		{
			item = ui->searchPaths->item(row);
			paths << item->text();
		}
		project->setSearchDirectories(paths);
	}
}

void ProjectSettings::setProject(Project *p)
{
	project = p;
	if(project) {
		ui->comboBoxTechnology->setCurrentIndex(ui->comboBoxTechnology->findData(project->getTechnology()));
		technologyActivated();

		ui->pathSynthesisScript->setText(project->getSynthesisScript());
		ui->pathSimulationScript->setText(project->getSimulationScript());

		setSearchPathList();
	}
}
