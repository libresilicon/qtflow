#include "schematicseditor.h"

SchematicsEditor::SchematicsEditor(QWidget *parent):
	QGraphicsView(parent),
	editScene(new QSchematicsScene(this)),
	filePath(QString()),
	schematicsdata(NULL)
{
	setScene(editScene);
}

void SchematicsEditor::loadFile(QString file)
{
	int x, y, w, h;
	//QString filedest;
	//QTemporaryDir temporaryDir;
	filePath = file;
	if(schematicsdata) delete schematicsdata;
	schematicsdata = new schematics::SchematicsData(file);

	x = schematicsdata->getLowerX();
	y = schematicsdata->getLowerY();
	w = schematicsdata->getUpperX()-x;
	h = schematicsdata->getUpperY()-y;

	if(w<this->width()) w = this->width();
	if(h<this->height()) h = this->height();

	//editScene->setGridSize(10);
	//editScene->setSceneRect(x,y,w,h);

	addWires();

	editScene->update();
}

void SchematicsEditor::addWires()
{
	if(schematicsdata && editScene) {
		foreach(SchematicsWire w, schematicsdata->getWires()) {
			editScene->addWire(w.getName(),w.getPos1(),w.getPos2());
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
