#include "deflayouteditor.h"

DEFLayoutEditor::DEFLayoutEditor(QWidget *parent) :
	GenericLayoutEditor(parent),
	defdata(NULL)
{
	editScene->setBackgroundBrush(Qt::white);

	setScene(editScene);
	//setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers |QGL::DirectRendering)));
	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	setInteractive(true);
	setDragMode(QGraphicsView::ScrollHandDrag);
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
	w = defdata->getUpperX()-x;
	h = defdata->getUpperY()-y;

	editScene->setSceneRect(x,y,w,h);

	addMacroInstances();
	addSignalWires();
	addContactPins();

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
		editScene->addPad(p.m_name, p.m_signal, p.m_layer, p.m_x, p.m_y, p.m_w, p.m_h);
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
				editScene->addWire(net,route.getLayer(),p1,p2);
			}
			p1 = p2;
		}
		if(route.getViaName()!=QString()) {
			editScene->addVia(net,route.getViaName(),p1);
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
					editScene->addWire(net,route.getLayer(),p1,p2);
				}
				p1 = p2;
			}
			if(route.getViaName()!=QString()) {
				editScene->addVia(net,route.getViaName(),p1);
			}
		}
	}
}

void DEFLayoutEditor::addMacroInstances()
{
	qreal count = 0;
	qreal angle = 0;
	QVector<def::DEFModuleInfo> mods = defdata->getModules();
	foreach (def::DEFModuleInfo e, mods) {
		// adding boxes for macros
		editScene->addMacro(e.macro_name, e.instance_name, e.x, e.y, e.orient);

		count++;
		qDebug() << "Added " << count << " of " << mods.count() << " (" << 100*(count/mods.count()) << "%)";
	}
}

void DEFLayoutEditor::saveFile()
{
}
