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
	ui->setupUi(this);
}

Templates::~Templates()
{
	if(ui) delete ui;
	if(project) delete project;
}

void Templates::on_buttonBox_accepted()
{
	QString path;
	QString name;
	bool ok;

	name = QInputDialog::getText(this, tr("New project"), tr("Project name:"), QLineEdit::Normal, QString(), &ok);
	if (ok && !name.isEmpty()) {
		path = QFileDialog::getExistingDirectory(this, tr("Open Directory..."), ".", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

		if (path == QString())
			return;

		if(project) delete project;
		project = new Project(settings, path+'/'+name+".pro", mainContext);

		foreach(QListWidgetItem *w, ui->listWidget->selectedItems()) {
			if(w->text()=="Mixed signal asic") {
			} else if(w->text()=="Analog macro cell") {
			} else {
				qDebug() << w->text();
			}
		}
	}
}

void Templates::on_buttonBox_rejected()
{
}
