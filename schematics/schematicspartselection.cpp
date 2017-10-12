#include "schematicspartselection.h"

SchematicsPartSelection::SchematicsPartSelection(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PartSelection),
	project(NULL)
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

void SchematicsPartSelection::setProject(Project *p)
{
	project = p;
	updatePartList();
}

void SchematicsPartSelection::updatePartList()
{
	QTreeWidgetItem *lib;

	ui->partList->header()->close();
	ui->partList->clear();
	m_libraries.clear();

	foreach(QString libname, project->getSchematicsLibraryNames()) {
		lib = new QTreeWidgetItem(ui->partList);
		lib->setText(0,libname);
		m_libraries.append(lib);
	}
}

void SchematicsPartSelection::on_partList_currentTextChanged(QString s)
{
}
