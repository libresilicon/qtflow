#include "templates.h"
#include "ui_templates.h"

#include "project.h"

#include <QFileDialog>
#include <QInputDialog>

Templates::Templates(QWidget *parent, QSettings *s, PythonQtObjectPtr *main) :
	QDialog(parent),
	ui(new Ui::Templates),
	mainContext(main),
	settings(s),
	project(NULL)
{
	QMap<QString,QString> ptypes;
	QListWidgetItem *w;
	QDomNodeList nl1, nl2;
	QDomElement e1, e2;
	QString techID;
	QString tech;

	ui->setupUi(this);

	connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));

	// bonded chips
	ptypes["asic_mixed"] = "Mixed signal ASIC";
	ptypes["asic_digital"] = "Pure digital ASIC";
	ptypes["asic_analog"] = "Analog signal ASIC";

	// cells:
	ptypes["cell_mixed"] = "Mixed signal macro cell";
	ptypes["cell_digital"] = "Pure digital macro cell";
	ptypes["cell_analog"] = "Analog signal macro cell";

	foreach(QString key, ptypes.keys()) {
		w = new QListWidgetItem;
		w->setData(Qt::UserRole, QVariant(key));
		w->setText(ptypes[key]);
		ui->listWidget->addItem(w);
	}

	/* reading process and technology info */
	settingsFileProcess = new QDomDocument();
	QFile file(":/process.xml");
	if(file.open(QIODevice::ReadOnly)) {
		settingsFileProcess->setContent(&file);
		file.close();
	}

	/* filling combo box with technology selection */
	nl1 = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl1.count(); i++) {
		e1 = nl1.at(i).toElement();
		techID = e1.attribute("xml:id");
		nl2 = e1.childNodes();
		for(int j = 0; j < nl2.count(); j++) {
			e2 = nl2.at(j).toElement();
			if(e2.tagName()=="title") {
				tech = e2.text();
			}
		}
		ui->comboTechnology->addItem(tech,techID);
	}
	on_comboTechnology_activated(0);
}

Templates::~Templates()
{
	if(ui) delete ui;
	if(project) delete project;
}

void Templates::on_buttonBox_accepted()
{
	QString path;
	QString ppath;
	QString technology;
	QString process;
	QString ptype;
	QString name;
	QVariant data;

	path = ui->projectPath->text();
	name = ui->projectName->text();

	if (name == QString())
		return;

	if (path == QString())
		return;

	ppath = path+'/'+name+".pro";

	if(project) delete project;
	project = new Project(settings, ppath, mainContext);

	ptype = "asic_mixed";
	foreach(QListWidgetItem *w, ui->listWidget->selectedItems()) {
		if (w != 0) {
			data = w->data(Qt::UserRole);
			ptype = data.toString();
		}
	}

	data = ui->comboTechnology->currentData();
	technology = data.toString();

	data = ui->comboProcess->currentData();
	process = data.toString();

	project->setProjectType(ptype);
	project->setTopLevel(name);
	project->setTechnology(technology);
	project->setProcess(process);

	emit(projectCreated(ppath));

	close();
}

void Templates::on_pushPath_clicked()
{
	QString f = QFileDialog::getExistingDirectory(this,"Folder name",".");
	if (f == QString())
		return;
	ui->projectPath->setText(f);
}

void Templates::on_comboTechnology_activated(int)
{
	QDomNodeList nl1, nl2, nl3;
	QDomElement e1, e2, e3;
	QVariant currentData = ui->comboTechnology->currentData();
	QString currentTech = currentData.toString();
	QString processNameID;
	QString processName;

	ui->comboProcess->clear();

	nl1 = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl1.count(); i++) {
		e1 = nl1.at(i).toElement();
		if(currentTech==e1.attribute("xml:id")) {
			nl2 = e1.childNodes();
			for(int j = 0; j < nl2.count(); j++) {
				e2 = nl2.at(j).toElement();
				if(e2.tagName()=="process") {
					processNameID = e2.attribute("xml:id");
					processName = processNameID;
					nl3 = e2.childNodes();
					for(int k = 0; k < nl2.count(); k++) {
						e3 = nl3.at(k).toElement();
						if(e3.tagName()=="description") {
							processName = e3.text();
						}
					}
					ui->comboProcess->addItem(processName,processNameID);
				}
			}
		}
	}
}