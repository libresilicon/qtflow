#include "templates.h"
#include "ui_templates.h"

#include "project.h"

Templates::Templates(QWidget *parent, QSettings *s, PythonQtObjectPtr main) :
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
	ui->libraryInfoBox->setVisible(false);

	connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));

	// bonded chips
	ptypes["asic"] = "ASIC (with bond-out)";

	// cells:
	ptypes["macro_cell"] = "Macro cell (without bond-out/ESD)";

	foreach(QString key, ptypes.keys()) {
		w = new QListWidgetItem;
		w->setData(Qt::UserRole, QVariant(key));
		w->setText(ptypes[key]);
		ui->listWidget->addItem(w);
	}

	/* reading process and technology info */
	settingsFileProcess = new QDomDocument();
	QFile file(QDir(settings->value("tech_path").toString()).filePath("process.xml"));
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

void Templates::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *b)
{
	QStringList filter;
	QVariant d;
	QString s;

	filter << "macro_cell";

	if(current != 0) {
		d = current->data(Qt::UserRole);
		s = d.toString();
		if(filter.contains(s)) {
			ui->libraryInfoBox->setVisible(true);
		} else {
			ui->libraryInfoBox->setVisible(false);
		}
	}
}

void Templates::on_buttonBox_accepted()
{
	QString path;
	QString ppath;
	QString technology;
	QString process;
	QString ptype;
	QString name;
	QString liberty;
	QString lef;
	QString symbols;
	QVariant data;

	path = ui->projectPath->text();
	name = ui->projectName->text();

	if (name == QString())
		return;

	if (path == QString())
		return;

	ppath = QDir(path).filePath(name+".pro");

	if(project) delete project;
	project = new Project(settings, ppath, mainContext);

	ptype = "asic";
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

	liberty = ui->libertyPath->text();
	lef = ui->libraryPath->text();
	symbols = ui->schematicsPath->text();

	project->setProjectType(ptype);
	project->setTopLevel(name);
	project->setTechnology(technology);
	project->setProcess(process);

	if(ptype=="macro_cell") {
		project->setSyncLEF(lef);
		project->setSyncLiberty(liberty);
		project->setSyncSymbols(symbols);
	}

	project->createFiles();

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
