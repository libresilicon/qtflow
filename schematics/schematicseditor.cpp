#include "schematicseditor.h"

SchematicsEditor::SchematicsEditor(QWidget *parent):
	QGraphicsView(parent),
	editScene(new QSchematicsScene(this)),
	filePath(QString()),
	schematicsdata(NULL)
{
}

void SchematicsEditor::loadFile(QString file)
{
	//int x, y, w, h;
	//QString filedest;
	//QTemporaryDir temporaryDir;
	filePath = file;
	if(schematicsdata) delete schematicsdata;
	schematicsdata = new schematics::SchematicsData(file);

	//x = schematicsdata->getLowerX();
	//y = schematicsdata->getLowerY();
	//w = schematicsdata->getUpperX()-x;
	//h = schematicsdata->getUpperY()-y;

	//if(w<this->width()) w = this->width();
	//if(h<this->height()) h = this->height();

	//editScene->setGridSize(10);
	//editScene->setSceneRect(x,y,w,h);

	//addMacroInstances();
	//addRectangles();

	editScene->update();
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
