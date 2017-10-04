#include "qlayoutscene.h"

QLayoutScene::QLayoutScene(QObject *parent) :
	QGraphicsScene(parent),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	lefdata(NULL),
	recentRectangle(NULL)
{}

QLayoutScene::QLayoutScene(const QRectF &sceneRect, QObject *parent) :
	QGraphicsScene(sceneRect, parent),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	lefdata(NULL),
	recentRectangle(NULL)
{}

QLayoutScene::QLayoutScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
	QGraphicsScene(x, y, width, height, parent),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	lefdata(NULL),
	recentRectangle(NULL)
{}

void QLayoutScene::setProject(Project *p)
{
	project = p;
}

void QLayoutScene::setLEF(lef::LEFData *d)
{
	lefdata = d;
}

void QLayoutScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	switch(recentOperation) {

		case DRAWING_OPERATION_RECTANGLE:
			if(activeLayer=="") return; // no layer selected
			lastOrig = event->scenePos();
			recentRectangle = new QLayoutRectItem(lastOrig.x(), lastOrig.y(), 1, 1);
			recentRectangle->setVisible(true);
			recentRectangle->setFlag(QGraphicsItem::ItemIsMovable, true);
			if(project) recentRectangle->setColor(project->colorMat(activeLayer));
			addItem(recentRectangle);
			break;

		case DRAWING_OPERATION_DRAG:
			if(!recentRectangle) {
				if(activeLayer=="") return; // no layer selected
				lastOrig = event->scenePos();
				foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
					if(!m->isLocked()) if(m->contains(lastOrig)) {
						recentRectangle = m;
						recentRectangle->setCursor(QCursor(Qt::ClosedHandCursor));
						lastRectOrig = recentRectangle->pos();
					}
				}
			}
			break;

		case DRAWING_OPERATION_CUT_OUT:
			if(!recentRectangle) {
				if(activeLayer=="") return; // no layer selected
				lastOrig = event->scenePos();
				foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
					if(!m->isLocked()) if(m->contains(lastOrig)) {
						recentRectangle = m;
						lastRectOrig = recentRectangle->pos();
						recentRectangle->setCutOutStart(lastOrig.x(),lastOrig.y());
					}
				}
			}
			break;

		default:
			qDebug() << "QLayoutScene::" << __FUNCTION__;
			break;
	}
}

void QLayoutScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qreal dx, dy;
	QPointF pt;

	pt = event->scenePos();
	switch(recentOperation) {

		case DRAWING_OPERATION_RECTANGLE:
			if(recentRectangle) {
				dx=pt.x()-lastOrig.x();
				dy=pt.y()-lastOrig.y();
				recentRectangle->setRect(lastOrig.x(),lastOrig.y(),dx,dy);;
				update();
			}
			break;

		case DRAWING_OPERATION_DRAG:
			if(recentRectangle) {
				dx=pt.x()-lastOrig.x();
				dy=pt.y()-lastOrig.y();
				recentRectangle->setPos(lastRectOrig.x()+dx,lastRectOrig.y()+dy);
				update();
			}
			break;

		case DRAWING_OPERATION_CUT_OUT:
			if(recentRectangle) {
				dx=pt.x()-lastOrig.x();
				dy=pt.y()-lastOrig.y();
				recentRectangle->updateRecentCutOut(dx,dy);
				update();
			}
			break;

		default:
			qDebug() << "QLayoutScene::" << __FUNCTION__;
			break;
	}
}

void QLayoutScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	switch(recentOperation) {

		case DRAWING_OPERATION_RECTANGLE:
			if(activeLayer=="") return; // no layer selected
			layer_rects[activeLayer].append(recentRectangle);
			recentRectangle = NULL;
			break;

		case DRAWING_OPERATION_DRAG:
			if(recentRectangle) {
				recentRectangle->setCursor(QCursor(Qt::ArrowCursor));
				recentRectangle = NULL;
				update();
			}
			break;

		case DRAWING_OPERATION_CUT_OUT:
			if(recentRectangle) {
				recentRectangle = NULL;
				update();
			}
			break;

		default:
			qDebug() << "QLayoutScene::" << __FUNCTION__;
			break;
	}
	Q_UNUSED(event);
}

void QLayoutScene::setDrawingOperation(drawing_operations o)
{
	recentOperation = o;
}

void QLayoutScene::setActiveLayer(QString layer)
{
	activeLayer = layer;
}

void QLayoutScene::setVisibleLayers(QStringList l)
{
	visibleLayers = l;
	redraw();
}

void QLayoutScene::redraw()
{
	QGraphicsRectItem *m;
	QLayoutRectItem *w;
	QGraphicsTextItem *t;
	bool visible;

	visible = true;
	foreach(QString layerN, macro_wires.keys()) {
		visible = visibleLayers.contains(layerN);
		foreach(m, macro_wires[layerN]) {
			m->setVisible(visible);
		}
	}

	visible = true;
	foreach(QString layerN, layer_rects.keys()) {
		visible = visibleLayers.contains(layerN);
		foreach(w, layer_rects[layerN]) {
			w->setVisible(visible);
		}
	}

	visible = visibleLayers.contains("comment");
	foreach(t, macro_texts) {
		t->setVisible(visible);
	}

	update();
}

void QLayoutScene::addWire(QString layer, int x, int y, int w, int h)
{
}

void QLayoutScene::addRectangle(QString layer, int x, int y, int w, int h)
{
	QLayoutRectItem *r = new QLayoutRectItem(x, y, w, h);
	r->setVisible(true);
	if(project) r->setColor(project->colorMat(layer));
	addItem(r);
	update();

	layer_rects[layer].append(r);
}

void QLayoutScene::addMacro(QString module_name, QString instance, int x, int y, int w, int h)
{
	lef::LEFPort *port;
	lef::LEFLayer *layer;
	lef::LEFMacro *macro;
	lef::LEFPin *pin;
	QColor color;
	QString layer_name;

	QGraphicsRectItem *mw;
	QLayoutMacroItem *mi;

	mi = new QLayoutMacroItem(x,y,w,h);
	mi->setVisible(true);

	// fill in library content:
	if(lefdata) if(lefdata->isDefinedMacro(module_name)) {
		macro = lefdata->getMacro(module_name);
		macro->scaleMacro(w, h);

		foreach(pin, macro->getPins()) {
			port = pin->getPort();
			foreach(layer, port->getLayers()) {
				layer_name = layer->getName();
				color = project->colorMat(layer_name);
				foreach(lef::rect_t rect, layer->getRects()) {
					mw = new QGraphicsRectItem(rect.x+x, rect.y+y, rect.w, rect.h, mi);
					mw->setBrush(QBrush(color));
					mw->setVisible(true);
					macro_wires[layer_name].append(mw);
				}
			}
		}
		foreach (layer, macro->getObstruction()->getLayers()) {
			layer_name = layer->getName();
			color = project->colorMat(layer_name);
			foreach(lef::rect_t rect, layer->getRects()) {
				mw = new QGraphicsRectItem(rect.x+x, rect.y+y, rect.w, rect.h, mi);
				mw->setBrush(QBrush(color));
				mw->setVisible(true);
				macro_wires[layer_name].append(mw);
			}
		}
	}

	addItem(mi);
	macros.append(mi);

	update();
}

QStringList QLayoutScene::getLayers()
{
	return layer_rects.keys();
}

QVector<QLayoutRectItem*> QLayoutScene::getRectangles(QString n)
{
	return (layer_rects.contains(n))?layer_rects[n]:QVector<QLayoutRectItem*>();
}
