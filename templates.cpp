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
	ui->setupUi(this);

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

	if (path == QString())
		return;

	ppath = path+'/'+name+".pro";

	if(project) delete project;
	project = new Project(settings, ppath, mainContext);

	foreach(QListWidgetItem *w, ui->listWidget->selectedItems()) {
		if (w != 0) {
			data = w->data(Qt::UserRole);
			ptype = data.toString();
			project->setProjectType(ptype);
			project->setTopLevel(name);
		}
	}
	emit(projectCreated(ppath));
}

void Templates::on_buttonBox_rejected()
{
}
