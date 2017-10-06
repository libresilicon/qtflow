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
}

void DEFLayoutEditor::mousePressEvent(QMouseEvent * e)
{
	double rad = 10;
	QPointF pt = mapToScene(e->pos());
	editScene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));
	QTextStream(stdout) << "Drawing here " << "\t x:" << pt.x() << "\t y:" << pt.y()  << '\n';
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

	addMacroInstances();
	//addRectangles();

	editScene->setGridSize(10);
	editScene->setSceneRect(x,y,w,h);

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
	QVector<def::DEFModuleInfo> mods = defdata->getModules();
	foreach (def::DEFModuleInfo e, mods) {
		// adding boxes for macros
		editScene->addMacro(e.macro_name, e.instance_name, e.x, e.y);
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
