#include "schematicseditor.h"

SchematicsEditor::SchematicsEditor(QWidget *parent):
	QGraphicsView(parent),
	filePath(QString()),
	schematicsdata(NULL),
	editScene(NULL),
	partSelection(new SchematicsPartSelection(this)),
	libraryEditor(new SchematicsLibraryEditor(this)),
	parametricPartSelection(new SchematicsParametricPartDialog(this))
{
	editScene = new QSchematicsScene(this);
	setScene(editScene);

	connect(partSelection,SIGNAL(insertPart(QString)),this,SLOT(onInsertPart(QString)));
}

void SchematicsEditor::onInsertPart(QString name)
{
	editScene->addPart(name,0,0);
}

void SchematicsEditor::setDrawingOperation(schematics_operations o)
{
	editScene->setDrawingOperation(o);
}

void SchematicsEditor::loadFile(QString file)
{
	qreal scalef = 0.1;
	QGraphicsRectItem *sheet;
	filePath = file;
	if(schematicsdata) delete schematicsdata;
	schematicsdata = new schematics::SchematicsData(file);

	//editScene->setGridSize(10);

	//editScene->setSceneRect(0,0,schematicsdata->getPaperWidth(),schematicsdata->getPaperHeigth());
	//editScene->setSceneRect(0,0,width(),height());

	sheet = editScene->setSheet(schematicsdata->getPaperWidth(),schematicsdata->getPaperHeigth());

	addWires();
	addParts();

	//sheet->setScale(scalef);

	//fitInView(sheet,Qt::KeepAspectRatio);
	//fitInView(sheet);

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
			if(p) editScene->addPart(p->getType(),p->getID(),p->x(),p->y());
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

void SchematicsEditor::showParametricPartSelection()
{
	parametricPartSelection->show();
}

void SchematicsEditor::showLibraryEditor()
{
	libraryEditor->show();
}
