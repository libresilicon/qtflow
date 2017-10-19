#include "schematicspartselection.h"

SchematicsPartSelection::SchematicsPartSelection(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PartSelection),
	project(NULL),
	partPreview(NULL)
{
	ui->setupUi(this);
	partPreview = new QGraphicsScene(ui->partPreview);
	ui->partPreview->setScene(partPreview);
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
	QTreeWidgetItem *part;
	QSchematicsPart* partWidget;
	symbol::SchematicsSymbol* partsymbol;

	ui->partList->header()->close();
	ui->partList->clear();
	m_libraries.clear();
	m_libraryParts.clear();
	m_libraryPartWidgets.clear();

	foreach(QString libname, project->getSchematicsLibraryNames()) {
		lib = new QTreeWidgetItem(ui->partList);
		lib->setText(0,libname);
		m_libraries.append(lib);
		foreach(QString partname, project->getSchematicsLibraryParts(libname)) {
			// adding symbol selection to list
			part = new QTreeWidgetItem(lib);
			part->setText(0,partname);
			m_libraryParts.append(part);

			// adding symbol to view
			partsymbol = project->getSchematicsPart(partname);
			partWidget = new QSchematicsPart(partsymbol, 0, 0);
			m_libraryPartWidgets[partname] = partWidget;
			partPreview->addItem(partWidget);
		}
	}
}

void SchematicsPartSelection::on_partList_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	QString previousItem, currentItem;

	currentItem = current->text(0);
	if(previous) {
		previousItem = previous->text(0);
		if(previousItem==currentItem) return;
	}

	foreach(QString key, m_libraryPartWidgets.keys()) {
		m_libraryPartWidgets[key]->setVisible(false);
	}

	if(m_libraryPartWidgets.contains(currentItem)) {
		ui->partPreview->fitInView(m_libraryPartWidgets[currentItem]);
		m_libraryPartWidgets[currentItem]->setVisible(true);
	}

	ui->partPreview->update();
}
