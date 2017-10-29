#include "qlayoutscene.h"

void QLayoutScene::basicInit()
{
	recentSelectRectangle->setZValue(1000);
	recentSelectRectangle->hide();
	recentSelectRectangle->setOpacity(0.25);
	recentSelectRectangle->setBrush(Qt::gray);
	addItem(recentSelectRectangle);

	connect(drcDialog,SIGNAL(runDRC()),this,SLOT(runDRC()));
}

QLayoutScene::QLayoutScene(QObject *parent) :
	QGraphicsScene(parent),
	drcDialog(new DRCSettings()),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	recentRectangle(NULL),
	recentSelectRectangle(new QGraphicsRectItem()),
	m_dragging(false)
{
	basicInit();
}

QLayoutScene::QLayoutScene(const QRectF &sceneRect, QObject *parent) :
	QGraphicsScene(sceneRect, parent),
	drcDialog(new DRCSettings()),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	recentRectangle(NULL),
	recentSelectRectangle(new QGraphicsRectItem()),
	m_dragging(false)
{
	basicInit();
}

QLayoutScene::QLayoutScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
	QGraphicsScene(x, y, width, height, parent),
	drcDialog(new DRCSettings()),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	recentRectangle(NULL),
	recentSelectRectangle(new QGraphicsRectItem()),
	m_dragging(false)
{
	basicInit();
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

/*QPointF QLayoutScene::snapGrid(QPointF pt) {
	qreal x, y;
	x = round(pt.x()/m_gridSize)*m_gridSize;
	y = round(pt.y()/m_gridSize)*m_gridSize;
	return QPointF(x,y);
}*/

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
	drcDialog->setProject(project);
}

void QLayoutScene::showDRC()
{
	drcDialog->show();
}

void QLayoutScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QMenu  menu;
	QAction *layerAction;

	//lastOrig = snapGrid(event->scenePos());
	lastOrig = event->scenePos();

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
								m->setSelected(false);
							}
						}
						m->setSelected(true);
						m->setDragMode(true);
						m->setCursor(QCursor(Qt::ClosedHandCursor));
					}
				}
			}
			break;

		case DRAWING_OPERATION_SELECT:
			if(activeLayer=="") return; // no layer selected
			foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
				m->setSelected(false);
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

		case DRAWING_OPERATION_SELECT_LAYER:
			menu.clear();
			foreach(QString k, layer_rects.keys()) {
				foreach(QLayoutRectItem *m, layer_rects[k]) {
					if(m->contains(lastOrig)) {
						layerAction = menu.addAction(k);
						break;
					}
				}

			}
			foreach(QString k, layer_gds.keys()) {
				foreach(QGraphicsPolygonItem *m, layer_gds[k]) {
					if(m->contains(lastOrig)) {
						layerAction = menu.addAction(k);
						break;
					}
				}
			}
			foreach(QString k, macro_wires.keys()) {
				foreach(QGraphicsRectItem *m, macro_wires[k]) {
					if(m->contains(lastOrig)) {
						layerAction = menu.addAction(k);
						break;
					}
				}
			}

			menu.exec(event->screenPos());
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

	//pt = snapGrid(event->scenePos());
	pt = event->scenePos();

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
					m->setSelected(true);
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

	//pt = snapGrid(event->scenePos());
	pt = event->scenePos();

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
				m->setDragMode(false);
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

void QLayoutScene::onVisibleLayersChanged(QStringList l)
{
	m_visibleLayers = l;
	redraw();
}

void  QLayoutScene::runDRC(QGraphicsRectItem *m)
{
}

void  QLayoutScene::runDRC(QLayoutRectItem *w)
{
}

void  QLayoutScene::runDRC(QGraphicsPolygonItem *p)
{
}

void  QLayoutScene::runDRC()
{
	QString layerName;
	QGraphicsRectItem *m;
	QLayoutRectItem *w;
	QGraphicsPolygonItem *p;

	foreach(layerName, macro_wires.keys()) {
		foreach(m, macro_wires[layerName]) {
			runDRC(m);
		}
	}

	foreach(layerName, layer_gds.keys()) {
		foreach(p, layer_gds[layerName]) {
			runDRC(p);
		}
	}

	foreach(layerName, layer_rects.keys()) {
		foreach(w, layer_rects[layerName]) {
			runDRC(w);
		}
	}
}

void QLayoutScene::redraw()
{
	QString layerName;
	QString altName;
	QGraphicsRectItem *m;
	QLayoutRectItem *w;
	QGraphicsTextItem *t;
	QGraphicsPolygonItem *p;
	bool visible;

	visible = true;
	foreach(layerName, macro_wires.keys()) {
		visible = m_visibleLayers.contains(layerName);
		foreach(m, macro_wires[layerName]) {
			m->setVisible(visible);
		}
	}

	visible = true;
	foreach(layerName, layer_gds.keys()) {
		visible = m_visibleLayers.contains(layerName);
		foreach(p, layer_gds[layerName]) {
			p->setVisible(visible);
		}
	}

	visible = true;
	foreach(layerName, layer_rects.keys()) {
		visible = m_visibleLayers.contains(layerName);
		foreach(w, layer_rects[layerName]) {
			w->setVisible(visible);
		}
	}

	//visible = visibleLayers.contains("comment");
	//foreach(t, macro_texts) {
	//	t->setVisible(visible);
	//}

	update();
}

void QLayoutScene::addRectangle(QString layer, int x, int y, int w, int h)
{
	QLayoutRectItem *r;

	r = new QLayoutRectItem(x, y, w, h);

	r->setVisible(true);
	if(project) r->setColor(project->colorMat(layer));
	addItem(r);
	update();

	layer_rects[layer].append(r);
	emit(registerLayer(layer));
}

void QLayoutScene::addMacro(QString macro_name, QString instance_name, int x, int y)
{
	lef::LEFMacro *macro;

	double w, h;

	// fill in library content:
	if(project) if(project->isDefinedMacro(macro_name)) {
		macro = project->getMacro(macro_name);
		w = macro->getWidth();
		h = macro->getHeight();

		addMacro(macro_name, instance_name, x, y, w, h);

	} else {
		qDebug() << macro_name << "Macro not defined";
	}

	update();
}

void QLayoutScene::addMacro(QString macro_name, QString instance_name, int x, int y, int w, int h)
{
	QColor color;
	QString layer_name;

	QGraphicsRectItem *mw;
	QLayoutMacroItem *mi;
	QGraphicsPolygonItem *polygon;
	QGraphicsRectItem *gdsbox;


	mi = new QLayoutMacroItem(x, y, w, h);
	mi->setVisible(true);
	mi->setOpacity(0.25);

	// fill in library content from LEF:
	lef::LEFPort *port;
	lef::LEFLayer *layer;
	lef::LEFMacro *macro;
	lef::LEFPin *pin;

	if(project) if(project->isDefinedMacro(macro_name)) {
		macro = project->getMacro(macro_name);
		macro->scaleMacro(w, h);

		foreach(pin, macro->getPins()) {
			port = pin->getPort();
			foreach(layer, port->getLayers()) {
				layer_name = layer->getName();
				color = project->colorMat(layer_name);
				foreach(lef::rect_t rect, layer->getRects()) {
					mw = new QGraphicsRectItem((rect.x+x), (rect.y+y), (rect.w), (rect.h), mi);
					mw->setBrush(QBrush(color));
					mw->setVisible(true);
					macro_wires[layer_name].append(mw);
				}
				emit(registerLayer(layer_name));
			}
		}
		foreach (layer, macro->getObstruction()->getLayers()) {
			layer_name = layer->getName();
			color = project->colorMat(layer_name);
			foreach(lef::rect_t rect, layer->getRects()) {
				mw = new QGraphicsRectItem((rect.x+x), (rect.y+y), (rect.w), (rect.h), mi);
				mw->setBrush(QBrush(color));
				mw->setVisible(true);
				macro_wires[layer_name].append(mw);
			}
			emit(registerLayer(layer_name));
		}
	}

	// fill in GDS data:
	GDSCell* cell;

	if(project) if(project->isDefinedGDSMacro(macro_name)) {
		cell = project->getGDSMacro(macro_name);
		if(cell) {
			cell->setRectangle(x, y, w, h);
			foreach(GDSBoundary *b, cell->getBoundaries()) {
				layer_name = project->layerNameFromCIF(b->getLayerIndex());
				if(layer_name==QString()) {
					layer_name = project->layerNameFromDStyle(b->getLayerIndex());
				}
				if(layer_name==QString()) {
					qDebug() << "Couldn't map layer " << b->getLayerIndex();
				} else {
					gdsbox = new QGraphicsRectItem(x, y, w, h, mi);
					polygon = new QGraphicsPolygonItem(gdsbox);
					color = project->colorMat(layer_name);
					polygon->setBrush(QBrush(color));
					polygon->setPolygon(b->getPolygon());
					layer_gds[layer_name].append(polygon);
					emit(registerLayer(layer_name));
				}
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
