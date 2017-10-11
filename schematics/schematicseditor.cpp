#include "schematicseditor.h"

SchematicsEditor::SchematicsEditor(QWidget *parent):
	QGraphicsView(parent),
	editScene(new QSchematicsScene(this)),
	filePath(QString()),
	schematicsdata(NULL),
	project(NULL),
	partSelection(new SchematicsPartSelection(this)),
	libraryEditor(new SchematicsLibraryEditor(this))
{
	setScene(editScene);
}

void SchematicsEditor::setProject(Project *p)
{
	project = p;
	editScene->setProject(project);
	partSelection->setProject(project);
}

void SchematicsEditor::loadFile(QString file)
{
	filePath = file;
	if(schematicsdata) delete schematicsdata;
	schematicsdata = new schematics::SchematicsData(file);

	//editScene->setGridSize(10);

	addWires();
	addParts();

	// add frame:
	editScene->addRect(0,0,schematicsdata->getPaperWidth(),schematicsdata->getPaperHeigth());
	scale(0.1,0.1);

	editScene->update();
}

void SchematicsEditor::addWires()
{
	QPointF p1, p2;
	qreal x1, y1, x2, y2;

	if(schematicsdata && editScene) {
		foreach(SchematicsWire *w, schematicsdata->getWires()) {
			p1 = w->getPos1();
			p2 = w->getPos2();
			x1 = p1.x();
			y1 = p1.y();
			x2 = p2.x();
			y2 = p2.y();
			editScene->addWire(w->getName(), x1, y1, x2, y2);
		}
	}
}

void SchematicsEditor::addParts()
{
	if(schematicsdata && editScene) {
		foreach(SchematicsPart *p, schematicsdata->getParts()) {
			qDebug() << "Adding part " << p->getName();
			qDebug() << "Adding part " << p->getType();
			editScene->addPart(p->getName(),p->getType(),p->x(),p->y());
		}
	}
}

void SchematicsEditor::saveFile()
{
}

QString SchematicsEditor::getFilePath()
{
	return filePath;
}

bool SchematicsEditor::changes()
{
	return false;
}

void SchematicsEditor::zoomIn()
{
	scale(1.1, 1.1);
	editScene->update();
}

void SchematicsEditor::zoomOut()
{
	scale(0.9, 0.9);
	editScene->update();
}

void SchematicsEditor::showPartSelection()
{
	partSelection->show();
}

void SchematicsEditor::showLibraryEditor()
{
	libraryEditor->show();
}
