#include "deflayouteditor.h"

DEFLayoutEditor::DEFLayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	defdata(NULL),
	project(NULL),
	filePath(QString()),
	editScene(new QLayoutScene(this)),
	m_scale(1)
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
	m_scale = project->getSmallestUnit();
	m_scale /= defdata->getDistanceUnit();

	x = defdata->getLowerX();
	y = defdata->getLowerY();
	w = defdata->getUpperX()-x;
	h = defdata->getUpperY()-y;

	//x *= 10;
	//y *= 10;
	//w *= 10;
	//h *= 10;

	//if(w<this->width()) w = this->width();
	//if(h<this->height()) h = this->height();

	//editScene->setGridSize(defdata->getDistanceUnit());
	//editScene->setSceneRect(x/100,y/100,w/100,h/100);

	r = editScene->addRect(x*m_scale,y*m_scale,w*m_scale,h*m_scale);
	pen = r->pen();
	pen.setWidth(100);
	r->setPen(pen);

	addMacroInstances();
	addSignalWires();
	addContactPins();
	//addRectangles();
	//fitInView(r);

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
		editScene->addPad(p.m_name, p.m_signal, p.m_layer, p.m_x*m_scale, p.m_y*m_scale, p.m_w*m_scale, p.m_h*m_scale);
	}
}

void DEFLayoutEditor::addSignalWires()
{
	DEFRouteInfo route;
	QVector<QPointF> points;
	QMap<QString,DEFRouteInfo> routes;
	QMap<QString,QVector<DEFRouteInfo>> sroutl;
	QPointF p1, p2;

	routes = defdata->getMainRoutes();
	foreach(QString net, routes.keys()) {
		route = routes[net];
		points = route.getPoints();
		if(points.count()<1) continue;
		p1 = points.at(0);
		foreach(p2, points) {
			if(p1!=p2) {
				editScene->addWire(net,route.getLayer(),p1*m_scale,p2*m_scale);
			}
			p1 = p2;
		}
		if(route.getViaName()!=QString()) {
			editScene->addVia(net,route.getViaName(),p1*m_scale);
		}
	}

	sroutl = defdata->getSecondaryRoutes();
	foreach(QString net, sroutl.keys()) {
		foreach (route, sroutl[net]) {
			points = route.getPoints();
			if(points.count()<1) continue;
			p1 = points.at(0);
			foreach(p2, points) {
				if(p1!=p2) {
					editScene->addWire(net,route.getLayer(),p1*m_scale,p2*m_scale);
				}
				p1 = p2;
			}
			if(route.getViaName()!=QString()) {
				editScene->addVia(net,route.getViaName(),p1*m_scale);
			}
		}
	}
}

void DEFLayoutEditor::addMacroInstances()
{
	qreal count = 0;
	qreal x, y;
	QVector<def::DEFModuleInfo> mods = defdata->getModules();
	foreach (def::DEFModuleInfo e, mods) {
		// adding boxes for macros
		x = e.x;
		y = e.y;
		editScene->addMacro(e.macro_name, e.instance_name, x*m_scale, y*m_scale, e.orient);

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
