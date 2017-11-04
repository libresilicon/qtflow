#include "deflayouteditor.h"

DEFLayoutEditor::DEFLayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	defdata(NULL),
	project(NULL),
	filePath(QString()),
	editScene(new QLayoutScene(this))
{
	editScene->setBackgroundBrush(Qt::white);

	setScene(editScene);
	setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));
	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void DEFLayoutEditor::loadFile(QString file)
{
	filePath = file;
	QGraphicsRectItem *r;
	QPen pen;

	int x, y, w, h;

	filePath = file;
	if(defdata) delete defdata;
	defdata = new def::DEFData(file);

	x = defdata->getLowerX();
	y = defdata->getLowerY();
	w = defdata->getUpperX();
	h = defdata->getUpperY();

	//x *= 10;
	//y *= 10;
	//w *= 10;
	//h *= 10;

	//if(w<this->width()) w = this->width();
	//if(h<this->height()) h = this->height();

	//editScene->setGridSize(defdata->getDistanceUnit());
	//editScene->setSceneRect(x/100,y/100,w/100,h/100);

	r = editScene->addRect(x,y,w,h);
	pen = r->pen();
	pen.setWidth(100);
	r->setPen(pen);

	addMacroInstances();
	addContactPins();
	//addRectangles();

	//scale(0.1,0.1);

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

void DEFLayoutEditor::addContactPins()
{
	QVector<def::DEFDataPin> pins = defdata->getPins();
	foreach (def::DEFDataPin p, pins) {
		qDebug() << "Added pin " << p.m_name;
		editScene->addPad(p.m_name,p.m_signal,p.m_layer, p.m_x, p.m_y, p.m_w, p.m_h);
	}
}

void DEFLayoutEditor::addMacroInstances()
{
	qreal count = 0;
	qreal scale;
	qreal x, y;
	//scale = defdata->getDistanceUnit();
	scale = 1;
	QVector<def::DEFModuleInfo> mods = defdata->getModules();
	foreach (def::DEFModuleInfo e, mods) {
		// adding boxes for macros
		x = e.x;
		y = e.y;
		editScene->addMacro(e.macro_name, e.instance_name, x, y);

		count++;
		qDebug() << "Added " << count << " of " << mods.count() << " (" << 100*(count/mods.count()) << "%)";
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
