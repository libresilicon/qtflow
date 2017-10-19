#include "qlayoutscene.h"

QLayoutScene::QLayoutScene(QObject *parent) :
	QGraphicsScene(parent),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	recentRectangle(NULL),
	recentSelectRectangle(new QGraphicsRectItem()),
	m_dragging(false),
	m_gridSize(40),
	m_scaleFactor(1)
{
	recentSelectRectangle->setZValue(1000);
	recentSelectRectangle->hide();
	recentSelectRectangle->setOpacity(0.25);
	recentSelectRectangle->setBrush(Qt::gray);
	addItem(recentSelectRectangle);
}

QLayoutScene::QLayoutScene(const QRectF &sceneRect, QObject *parent) :
	QGraphicsScene(sceneRect, parent),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	recentRectangle(NULL),
	recentSelectRectangle(new QGraphicsRectItem()),
	m_dragging(false),
	m_gridSize(2),
	m_scaleFactor(1)
{
	recentSelectRectangle->setZValue(1000);
	recentSelectRectangle->hide();
	recentSelectRectangle->setOpacity(0.25);
	recentSelectRectangle->setBrush(Qt::gray);
	addItem(recentSelectRectangle);
}

QLayoutScene::QLayoutScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
	QGraphicsScene(x, y, width, height, parent),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	recentRectangle(NULL),
	recentSelectRectangle(new QGraphicsRectItem()),
	m_dragging(false),
	m_gridSize(40),
	m_scaleFactor(1)
{
	recentSelectRectangle->setZValue(1000);
	recentSelectRectangle->hide();
	recentSelectRectangle->setOpacity(0.25);
	recentSelectRectangle->setBrush(Qt::gray);
	addItem(recentSelectRectangle);
}


int QLayoutScene::countSelectedRectItems(QVector<QLayoutRectItem*> l)
{
	int ret = 0;
	QLayoutRectItem *m;
	foreach(m,l) {
		if(m->isSelected())
			ret++;
	}
	return ret;
}

void QLayoutScene::keyPressEvent(QKeyEvent *event)
{
	if(event->key()==Qt::Key_Delete) {
		if(activeLayer=="") return; // no layer selected
		QLayoutRectItem *m;
		QVector<QLayoutRectItem*> l = layer_rects[activeLayer];
		while(countSelectedRectItems(l)) {
			for(int i=0; i<l.count(); i++) {
				m = l.at(i);
				if(m->isSelected()) {
					removeItem(m);
					l.remove(i);
					break;
				}
			}
		}
		layer_rects[activeLayer] = l;
	}
}

QPointF QLayoutScene::snapGrid(QPointF pt) {
	qreal x, y;
	x = round(pt.x()/m_gridSize)*m_gridSize;
	y = round(pt.y()/m_gridSize)*m_gridSize;
	return QPointF(x,y);
}

/*void QLayoutScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	QVector<QLineF> lines;
	qreal left, top;

	left = int(rect.left())-(int(rect.left()) % m_gridSize);
	top = int(rect.top())-(int(rect.top()) % m_gridSize);
	for (qreal x = left; x < rect.right(); x += m_gridSize){
		lines.append(QLineF(QPointF(x,rect.top()),QPointF(x,rect.bottom())));
		for (qreal y = top; y < rect.bottom(); y += m_gridSize){
			lines.append(QLineF(QPointF(rect.left(),y),QPointF(rect.right(),y)));
		}
	}

	painter->setPen(QPen(QColor(200, 200, 255, 125)));
	painter->drawLines(lines.data(), lines.size());
}*/

void QLayoutScene::setProject(Project *p)
{
	project = p;
}

void QLayoutScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	lastOrig = snapGrid(event->scenePos());

	switch(recentOperation) {

		case DRAWING_OPERATION_RECTANGLE:
			if(activeLayer=="") return; // no layer selected
			recentRectangle = new QLayoutRectItem(lastOrig.x(), lastOrig.y(), 1, 1);
			recentRectangle->setVisible(true);
			recentRectangle->setFlag(QGraphicsItem::ItemIsMovable, true);
			if(project) recentRectangle->setColor(project->colorMat(activeLayer));
			addItem(recentRectangle);
			break;

		case DRAWING_OPERATION_DRAG:
			if(activeLayer=="") return; // no layer selected
			if(m_dragging) return;
			m_dragging = true;
			foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
				if(!m->isLocked()) {
					if(m->contains(lastOrig)||m->isSelected()) {
						if(!m->isSelected()) {
							foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
								m->unSelectItem();
							}
						}
						m->selectItem();
						m->startMoving();
						m->setCursor(QCursor(Qt::ClosedHandCursor));
					}
				}
			}
			break;

		case DRAWING_OPERATION_SELECT:
			if(activeLayer=="") return; // no layer selected
			foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
				m->unSelectItem();
			}

			recentSelectRectangle->setRect(QRectF(lastOrig.x(), lastOrig.y(), 1, 1));
			recentSelectRectangle->show();
			break;

		case DRAWING_OPERATION_CUT_OUT:
			if(!recentRectangle) {
				if(activeLayer=="") return; // no layer selected
				foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
					if(!m->isLocked()) if(m->contains(lastOrig)) {
						recentRectangle = m;
						recentRectangle->setCutOutStart(lastOrig.x(),lastOrig.y());
					}
				}
			}
			break;

		default:
			break;
	}
}

void QLayoutScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qreal dx, dy;
	QPointF pt;
	QRectF srect;

	pt = snapGrid(event->scenePos());

	switch(recentOperation) {

		case DRAWING_OPERATION_RECTANGLE:
			if(recentRectangle) {
				dx=pt.x()-lastOrig.x();
				dy=pt.y()-lastOrig.y();
				recentRectangle->setRect(lastOrig.x(),lastOrig.y(),dx,dy);;
			}
			update();
			break;

		case DRAWING_OPERATION_DRAG:
			if(activeLayer=="") return; // no layer selected
			if(!m_dragging) return;
			foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
				if(m->isSelected()) {
					dx=pt.x()-lastOrig.x();
					dy=pt.y()-lastOrig.y();
					m->updateMovingOffset(dx,dy);
				}
			}
			update();
			break;

		case DRAWING_OPERATION_SELECT:
			dx=pt.x()-lastOrig.x();
			dy=pt.y()-lastOrig.y();
			srect = QRectF(lastOrig.x(), lastOrig.y(), dx, dy);
			recentSelectRectangle->setRect(srect);

			if(activeLayer=="") return; // no layer selected
			foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
				if(srect.contains(m->offsetRect())) {
					m->selectItem();
				}
			}
			update();
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
			break;
	}
}

void QLayoutScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF pt;

	pt = snapGrid(event->scenePos());

	switch(recentOperation) {

		case DRAWING_OPERATION_RECTANGLE:
			if(activeLayer=="") return; // no layer selected
			layer_rects[activeLayer].append(recentRectangle);
			recentRectangle = NULL;
			update();
			break;

		case DRAWING_OPERATION_DRAG:
			if(activeLayer=="") return; // no layer selected
			foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
				m->setCursor(QCursor(Qt::ArrowCursor));
				m_dragging = false;
			}
			update();
			break;

		case DRAWING_OPERATION_SELECT:
			if(activeLayer=="") return; // no layer selected
			recentSelectRectangle->hide();
			update();
			break;

		case DRAWING_OPERATION_CUT_OUT:
			recentRectangle = NULL;
			update();
			break;

		default:
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

void QLayoutScene::setGridSize(int s) {
	m_gridSize = s;
	update();
}

int QLayoutScene::getScaleFactor()
{
	return m_scaleFactor;
}

void QLayoutScene::setScaleFactor(int s)
{
	QMatrix m;
	if(s > 0) {
		m_scaleFactor = s;
		m.scale(s, s);
		//setMatrix(m);
	}
}

void QLayoutScene::addWire(QString layer, int x, int y, int w, int h)
{
}

void QLayoutScene::addRectangle(QString layer, int x, int y, int w, int h)
{
	QLayoutRectItem *r;

	x*=m_scaleFactor;
	y*=m_scaleFactor;
	w*=m_scaleFactor;
	h*=m_scaleFactor;
	r = new QLayoutRectItem(x, y, w, h);

	r->setVisible(true);
	if(project) r->setColor(project->colorMat(layer));
	addItem(r);
	update();

	layer_rects[layer].append(r);
}

void QLayoutScene::addMacro(QString macro_name, QString instance_name, int x, int y)
{
	lef::LEFPort *port;
	lef::LEFLayer *layer;
	lef::LEFMacro *macro;
	lef::LEFPin *pin;
	QColor color;
	QString layer_name;

	QGraphicsRectItem *mw;
	QLayoutMacroItem *mi;

	double w, h;

	x*=m_scaleFactor;
	y*=m_scaleFactor;

	// fill in library content:
	if(project) if(project->isDefinedMacro(macro_name)) {
		macro = project->getMacro(macro_name);
		w = macro->getWidth();
		h = macro->getHeight();
		w*=m_scaleFactor;
		h*=m_scaleFactor;

		macro->scaleMacro(w,h);

		mi = new QLayoutMacroItem(x,y,w,h);
		mi->setVisible(true);

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

		addItem(mi);
		macros.append(mi);
	}

	update();
}

void QLayoutScene::addMacro(QString macro_name, QString instance_name, int x, int y, int w, int h)
{
	lef::LEFPort *port;
	lef::LEFLayer *layer;
	lef::LEFMacro *macro;
	lef::LEFPin *pin;
	QColor color;
	QString layer_name;

	QGraphicsRectItem *mw;
	QLayoutMacroItem *mi;

	x*=m_scaleFactor;
	y*=m_scaleFactor;
	w*=m_scaleFactor;
	h*=m_scaleFactor;

	mi = new QLayoutMacroItem(x,y,w,h);
	mi->setVisible(true);

	// fill in library content:
	if(project) if(project->isDefinedMacro(macro_name)) {
		macro = project->getMacro(macro_name);
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
