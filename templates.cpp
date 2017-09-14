#include "templates.h"
#include "ui_templates.h"

#include "project.h"

#include <QFileDialog>

Templates::Templates(QWidget *parent, QSettings *s, PythonQtObjectPtr *main) :
	QDialog(parent),
	ui(new Ui::Templates),
	mainContext(main),
	settings(s)
{
	ui->setupUi(this);
	connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(on_buttonBox_accepted()));
	connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(on_buttonBox_rejected()));
}

Templates::~Templates()
{
	delete ui;
	delete project;
}

void Templates::on_buttonBox_accepted()
{
	//QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory..."), ".", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	//if (path == QString())
	//	return;

	//if(project) delete project;
	//project = new Project(settings, path, mainContext);

	foreach(QListWidgetItem *w, ui->listWidget->selectedItems()) {
		if(w->text()=="Mixed signal asic") {
		} else if(w->text()=="Analog macro cell") {
		} else {
			qDebug() << w->text();
		}
	}
}

void Templates::on_buttonBox_rejected()
{

}
