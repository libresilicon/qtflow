#include "deflayouteditor.h"

DEFLayoutEditor::DEFLayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	defdata(NULL),
	lefdata(NULL),
	project(NULL),
	filePath(QString()),
	editScene(new QLayoutScene(this))
{
	editScene->setBackgroundBrush(Qt::white);

	setScene(editScene);
	//setRenderHint(QPainter::Antialiasing);
	//setCacheMode(QGraphicsView::CacheBackground);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void DEFLayoutEditor::loadFile(QString file)
{
	filePath = file;

	int x, y, w, h;
	QString filedest;
	QTemporaryDir temporaryDir;
	filePath = file;
	if(defdata) delete defdata;
	defdata = new def::DEFData(file);

	x = defdata->getLowerX();
	y = defdata->getLowerY();
	w = defdata->getUpperX()-x;
	h = defdata->getUpperY()-y;

	if(w<this->width()) w = this->width();
	if(h<this->height()) h = this->height();

	if(lefdata) delete lefdata;
	lefdata = new lef::LEFData();
	foreach(QString filename, project->getLibraryFiles()) {
		filedest = temporaryDir.path()+"/cells.lef";
		QFile::copy(filename, filedest);
		if(QFile(filedest).exists()) {
			lefdata->loadFile(filedest);
		}
	}
	editScene->setLEF(lefdata);

	editScene->setGridSize(10);
	//editScene->setSceneRect(x/100,y/100,w/100,h/100);

	editScene->setScaleFactor(100);
	addMacroInstances();
	//addRectangles();

	editScene->update();
}

void DEFLayoutEditor::addRectangles()
{
	/*rects_t layer;
	layer_rects_t rects = defdata->getRectangles();
	foreach(QString layerN, rects.keys()) {
		layer = rects[layerN];
		foreach (rect_t e, layer) {
			editScene->addRectangle(layerN, e.x1, e.y1, e.x2-e.x1, e.y2-e.y1);
		}
	}*/
}

void DEFLayoutEditor::addMacroInstances()
{
	double x, y;
	QVector<def::DEFModuleInfo> mods = defdata->getModules();
	foreach (def::DEFModuleInfo e, mods) {
		// adding boxes for macros
		x = e.x / 100;
		y = e.y / 100;
		editScene->addMacro(e.macro_name, e.instance_name, x, y);
	}
}

void DEFLayoutEditor::saveFile()
{
}

QString DEFLayoutEditor::getFilePath()
{
	return filePath;
}

bool DEFLayoutEditor::changes()
{
	return false;
}

void DEFLayoutEditor::setProject(Project *p)
{
	project = p;
	editScene->setProject(p);
}

void DEFLayoutEditor::zoomIn()
{
	scale(1.1, 1.1);
	editScene->update();
}

void DEFLayoutEditor::zoomOut()
{
	scale(0.9, 0.9);
	editScene->update();
}
