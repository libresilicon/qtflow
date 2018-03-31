#include "qlayoutscene.h"

void QLayoutScene::basicInit()
{
	m_gridSize = LAYOUT_SCALE_FACTOR;

	drcDialog = new DRCSettings();
	connect(drcDialog,SIGNAL(runDRC()),this,SLOT(runDRC()));

	cellManagerDialog = new QLayoutCellManager();
	connect(cellManagerDialog,SIGNAL(setGDS(QString,bool)),this,SLOT(setGDS(QString,bool)));
	connect(cellManagerDialog,SIGNAL(setLEF(QString,bool)),this,SLOT(setLEF(QString,bool)));

	m_recentOperation = DRAWING_OPERATION_NONE;
	m_recentRectangle = NULL;
	m_recentDistanceMeasure = NULL;
	m_dragging = false;
	m_techData = NULL;
	m_lambaValue = 1;
	m_lambaUnit = "";

	m_recentSelectRectangle = new QGraphicsRectItem();
	m_recentSelectRectangle->setZValue(1000);
	m_recentSelectRectangle->hide();
	m_recentSelectRectangle->setOpacity(0.25);
	m_recentSelectRectangle->setBrush(Qt::gray);
	addItem(m_recentSelectRectangle);
}

QLayoutScene::QLayoutScene(QObject *parent) :
	QGraphicsScene(parent)
{
	basicInit();
}

QLayoutScene::QLayoutScene(const QRectF &sceneRect, QObject *parent) :
	QGraphicsScene(sceneRect, parent)
{
	basicInit();
}

QLayoutScene::QLayoutScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
	QGraphicsScene(x, y, width, height, parent)
{
	basicInit();
}

void QLayoutScene::setLambdaUnit(QString s)
{
	m_lambaUnit = s;
}

void QLayoutScene::setLambdaValue(qreal i)
{
	m_lambaValue = i;
}

void QLayoutScene::setTechData(TechDataWrapper* t)
{
	m_techData = t;
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
		if(m_activeLayer=="") return; // no layer selected
		QLayoutRectItem *m;
		QVector<QLayoutRectItem*> l = layer_rects[m_activeLayer];
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
		layer_rects[m_activeLayer]=l;
	}
}

QPointF QLayoutScene::snapGrid(QPointF pt) {
	qreal x, y;
	x = round(pt.x()/m_gridSize)*m_gridSize;
	y = round(pt.y()/m_gridSize)*m_gridSize;
	return QPointF(x,y);
}

void QLayoutScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	QPen p;
	QColor color;
	QVector<QLineF> lines;
	qreal left, top;
	left = int(rect.left())-(int(rect.left()) % m_gridSize);
	top = int(rect.top())-(int(rect.top()) % m_gridSize);
	for (qreal x = 0; x < rect.right(); x += m_gridSize) {
		lines.append(QLineF(QPointF(x,rect.top()),QPointF(x,0)));
		painter->drawText(QPoint(x,0),QString::number(m_lambaValue*x/m_gridSize)+m_lambaUnit);
		for (qreal y = top; y < 0; y += m_gridSize) {
			lines.append(QLineF(QPointF(0,y),QPointF(rect.right(),y)));
			if(x==0) {
				painter->drawText(QPoint(x,y),QString::number(abs(m_lambaValue*y/m_gridSize))+m_lambaUnit);
			}
		}
	}
	color = QColor(200, 200, 255, 125);
	p = QPen(color);
	p.setCosmetic(true);
	painter->setPen(p);
	painter->drawLines(lines.data(), lines.size());
}

void QLayoutScene::refreshViaTable()
{
	QLayoutViaItem* vt;
	qreal w,h;
	QColor color;
	lef::LEFVia* v;
	m_viaTemplateMap.clear();
	/*if(project) {
		foreach(QString viaName, project->getViaList()) {
			v = project->getVia(viaName);
			if(v) {
				w = v->width();
				h = v->height();
				w *= project->getViaScale(viaName);
				h *= project->getViaScale(viaName);

				vt = new QLayoutViaItem(0,0,w,h);
				foreach(lef::LEFLayer* l, v->getLayers()) {
					//qDebug() << "Adding layer to via: " << l->getName();
					color = project->colorMat(l->getName());
					foreach(lef::rect_t r, l->getRects()) {
						vt->addRectangle(l->getName(),QBrush(color),QRectF(r.x,r.y,r.w,r.h));
					}
				}
				m_viaTemplateMap[viaName]=vt;
			}
		}
	}*/
}

void QLayoutScene::refreshMacroTable()
{
	return;
	//if(!project) {
	//	qDebug() << "No project set!";
	//	return;
	//}

	QStringList macroList;
	QString layer_name;
	QString pin_name;
	QColor color;
	QLayoutMacroItem* mi;
	QLayoutMacroPinItem* pi;
	qreal x,y,w,h;
	qreal scale;

	// fill in library content from LEF:
	lef::LEFPort *port;
	lef::LEFLayer *layer;
	lef::LEFMacro *macro;
	lef::LEFPin *pin;

	// fill in GDS data:
	GDSCell* cell;

	//macroList = project->getMacroList();
	qreal macro_count = 0;

	m_macroTemplateMap.clear();
	foreach(QString macroName, macroList) {
		x = 0;
		y = 0;
		w = 1;
		h = 1;

		/*if(project) {
			macro = project->getMacro(macroName);
			cell = project->getGDSMacro(macroName);
			if(macro) {
				w = macro->getWidth();
				h = macro->getHeight();
			}
			w*=m_baseUnit;
			h*=m_baseUnit;
		}*/

		mi = new QLayoutMacroItem(x, y, w, h);
		mi->setVisible(true);
		mi->setOpacity(0.25);
		mi->setMacroName(macroName);

		// fill in GDS data:
		/*if(project && cell && macro) {
		}*/

		m_macroTemplateMap[macroName]=mi;

		macro_count++;
		qDebug() << "Loaded macro " << macroName << " (" << 100*(macro_count/macroList.count()) << "% of the macros)";
	}
}

void QLayoutScene::showCellManager()
{
	cellManagerDialog->show();
}

void QLayoutScene::showDRC()
{
	drcDialog->show();
}

void QLayoutScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QMenu  menu;
	QAction *layerAction;
	m_lastOrig = snapGrid(event->scenePos());
	switch(m_recentOperation) {

		case DRAWING_OPERATION_RECTANGLE:
			if(m_activeLayer=="") return; // no layer selected
			if(m_activeLayer==ALL_LAYERS) return; // no layer selected
			m_recentRectangle = new QLayoutRectItem(m_lastOrig.x(), m_lastOrig.y(), 1, 1);
			m_recentRectangle->setVisible(true);
			m_recentRectangle->setFlag(QGraphicsItem::ItemIsMovable, true);
			if(m_techData) m_recentRectangle->setColor(m_techData->getLayerColor(m_activeLayer));
			addItem(m_recentRectangle);
			break;

		case DRAWING_OPERATION_DRAG:
			if(m_activeLayer=="") return; // no layer selected
			if(m_dragging) return;
			m_dragging = true;
			foreach(QString lay, layer_rects.keys()) {
				if((m_activeLayer!=ALL_LAYERS)&&(m_activeLayer!=lay)) continue;
				foreach(QLayoutRectItem *m, layer_rects[lay]) {
					if(!m->isLocked()) {
						if(m->contains(m_lastOrig)||m->isSelected()) {
							if(!m->isSelected()) {
								foreach(QLayoutRectItem *m, layer_rects[lay]) {
									m->setSelected(false);
								}
							}
							m->setSelected(true);
							m->setDragMode(true);
							m->setCursor(QCursor(Qt::ClosedHandCursor));
						}
					}
				}
			}
			break;

		case DRAWING_OPERATION_SELECT:
			if(m_activeLayer=="") return; // no layer selected
			foreach(QString lay, layer_rects.keys()) {
				if((m_activeLayer!=ALL_LAYERS)&&(m_activeLayer!=lay)) continue;
				foreach(QLayoutRectItem *m, layer_rects[lay]) {
					m->setSelected(false);
				}
			}
			m_recentSelectRectangle->setRect(QRectF(m_lastOrig.x(), m_lastOrig.y(), 1, 1));
			m_recentSelectRectangle->show();
			break;

		case DRAWING_OPERATION_CUT_OUT:
			if(!m_recentRectangle) {
				if(m_activeLayer=="") return; // no layer selected
				foreach(QString lay, layer_rects.keys()) {
					if((m_activeLayer!=ALL_LAYERS)&&(m_activeLayer!=lay)) continue;
					foreach(QLayoutRectItem *m, layer_rects[lay]) {
						if(!m->isLocked()) if(m->contains(m_lastOrig)) {
							m_recentRectangle = m;
							m_recentRectangle->setCutOutStart(m_lastOrig.x(),m_lastOrig.y());
						}
					}
				}
			}
			break;

		case DRAWING_OPERATION_SELECT_LAYER:
			menu.clear();
			foreach(QString lay, layer_rects.keys()) {
				if((m_activeLayer!=ALL_LAYERS)&&(m_activeLayer!=lay)) continue;
				foreach(QLayoutRectItem *m, layer_rects[lay]) {
					if(m->contains(m_lastOrig)) {
						layerAction = menu.addAction(lay);
						break;
					}
				}
			}

			menu.exec(event->screenPos());
			break;

		case DRAWING_OPERATION_DIMENSION:
			m_recentDistanceMeasure = new QLayoutDistanceMeasure();
			if(m_recentDistanceMeasure) {
				m_recentDistanceMeasure->setLine(m_lastOrig.x(), m_lastOrig.y(), m_lastOrig.x(), m_lastOrig.y());
				addItem(m_recentDistanceMeasure);
			}
			break;

		default:
			break;
	}
}

void QLayoutScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qreal dx, dy, d;
	QRectF srect;
	QPointF pt = snapGrid(event->scenePos());
	switch(m_recentOperation) {

		case DRAWING_OPERATION_RECTANGLE:
			if(m_recentRectangle) {
				dx=pt.x()-m_lastOrig.x();
				dy=pt.y()-m_lastOrig.y();
				if((abs(dx)>0)&&(abs(dy)>0))
					m_recentRectangle->setRect(m_lastOrig.x(),m_lastOrig.y(),dx,dy);;
			}
			break;

		case DRAWING_OPERATION_DRAG:
			if(m_activeLayer=="") return; // no layer selected
			if(!m_dragging) return;
			foreach(QString lay, layer_rects.keys()) {
				if((m_activeLayer!=ALL_LAYERS)&&(m_activeLayer!=lay)) continue;
				foreach(QLayoutRectItem *m, layer_rects[lay]) {
					if(m->isSelected()) {
						dx=pt.x()-m_lastOrig.x();
						dy=pt.y()-m_lastOrig.y();
						m->updateMovingOffset(dx,dy);
					}
				}
			}
			break;

		case DRAWING_OPERATION_SELECT:
			dx=pt.x()-m_lastOrig.x();
			dy=pt.y()-m_lastOrig.y();
			srect = QRectF(m_lastOrig.x(), m_lastOrig.y(), dx, dy);
			m_recentSelectRectangle->setRect(srect);
			if(m_activeLayer=="") return; // no layer selected
			foreach(QString lay, layer_rects.keys()) {
				if((m_activeLayer!=ALL_LAYERS)&&(m_activeLayer!=lay)) continue;
				foreach(QLayoutRectItem *m, layer_rects[lay]) {
					if(m) if(srect.contains(m->offsetRect())) {
						m->setSelected(true);
					}
				}
			}
			break;

		case DRAWING_OPERATION_CUT_OUT:
			if(m_recentRectangle) {
				dx=pt.x()-m_lastOrig.x();
				dy=pt.y()-m_lastOrig.y();
				m_recentRectangle->updateRecentCutOut(dx,dy);
			}
			break;

		case DRAWING_OPERATION_DIMENSION:
			if(m_recentDistanceMeasure) {
				m_recentDistanceMeasure->setLine(m_lastOrig.x(), m_lastOrig.y(), pt.x(), pt.y());
				dx=pt.x()-m_lastOrig.x();
				dy=pt.y()-m_lastOrig.y();
				d=sqrt(pow(dx,2)+pow(dy,2));
				m_recentDistanceMeasure->updateLabel(QString::number(d*m_lambaValue)+m_lambaUnit);
			}
			break;

		default:
			break;
	}
	update();
}

void QLayoutScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF pt = snapGrid(event->scenePos());
	switch(m_recentOperation) {

		case DRAWING_OPERATION_RECTANGLE:
			if(m_activeLayer=="") return; // no layer selected
			if(m_activeLayer==ALL_LAYERS) return; // no layer selected
			layer_rects[m_activeLayer].append(m_recentRectangle);
			m_recentRectangle = NULL;
			update();
			break;

		case DRAWING_OPERATION_DRAG:
			if(m_activeLayer=="") return; // no layer selected
			foreach(QString lay, layer_rects.keys()) {
				if((m_activeLayer!=ALL_LAYERS)&&(m_activeLayer!=lay)) continue;
				foreach(QLayoutRectItem *m, layer_rects[lay]) {
					m->setCursor(QCursor(Qt::ArrowCursor));
					m->setDragMode(false);
					m_dragging = false;
				}
			}
			update();
			break;

		case DRAWING_OPERATION_SELECT:
			if(m_activeLayer=="") return; // no layer selected
			m_recentSelectRectangle->hide();
			update();
			break;

		case DRAWING_OPERATION_CUT_OUT:
			m_recentRectangle = NULL;
			update();
			break;

		default:
			break;
	}
}

void QLayoutScene::setDrawingOperation(drawing_operations o)
{
	m_recentOperation = o;
}

void QLayoutScene::setActiveLayer(QString layer)
{
	m_activeLayer = layer;
	qDebug() << __FUNCTION__ << " setting layer " << layer;
}

void QLayoutScene::onVisibleLayersChanged(QStringList l)
{
	bool visible = true;
	foreach(QString layerName, layer_rects.keys()) {
		visible = l.contains(layerName);
		foreach(QLayoutRectItem* w, layer_rects[layerName]) {
			w->setVisible(visible);
		}
	}
	update();
}

void QLayoutScene::setGDS(QString name)
{
	GDSCell* cell;
	QColor color;
	QLayoutMacroItem* mi;
	QLayoutMacroPinItem* pi;
	QString layer_name;
	GDTData gdsFile(name);
	QVector<GDSCell*> cells = gdsFile.getCells();

	foreach(cell,cells) {
		if(cell) {
			foreach(mi,macros) {
				if(mi->getMacroName()==cell->getName()) {
					qDebug() << __FUNCTION__ << " Display GDS " << mi->getInstanceName();
					cell->setRectangle(mi->boundingRect().x(),mi->boundingRect().y(),mi->boundingRect().width(),mi->boundingRect().height());
					foreach(GDSBoundary *b, cell->getBoundaries()) {
						color = QColor("red");
						mi->addPolygon("poly", QBrush(color), b->getPolygon());
						emit(registerLayer("poly"));
					}
				}
			}
		}
	}
}

void QLayoutScene::setLEF(QString name)
{
	lef::LEFMacro* macro;
	lef::LEFPin* pin;
	lef::LEFPort* port;
	lef::LEFLayer* layer;
	QColor color;
	QLayoutMacroItem* mi;
	QLayoutMacroPinItem* pi;
	QString pin_name;
	QString layer_name;
	foreach(mi, macros) {
		if(mi) {
			if(mi->getInstanceName()==name) {
					/*if(project) {
						macro = project->getMacro(mi->getMacroName());
						if(macro) {
							qDebug() << __FUNCTION__ << " Display LEF " << mi->getInstanceName();
							qDebug() << __FUNCTION__ << " Display LEF " << name;
							foreach(pin, macro->getPins()) {
								pin_name = pin->getName();
								pi = mi->addPin(pin_name);
								port = pin->getPort();
								foreach(layer, port->getLayers()) {
									layer_name = layer->getName();
									color = project->colorMat(layer_name);
									foreach(lef::rect_t rect, layer->getRects()) {
										pi->addRectangle(layer_name, QBrush(color), QRectF(rect.x*m_baseUnit, rect.y*m_baseUnit, rect.w*m_baseUnit, rect.h*m_baseUnit));
									}
									emit(registerLayer(layer_name));
								}
							}
							foreach (layer, macro->getObstruction()->getLayers()) {
								layer_name = layer->getName();
								color = project->colorMat(layer_name);
								foreach(lef::rect_t rect, layer->getRects()) {
									mi->addRectangle(layer_name, QBrush(color), QRectF(rect.x*m_baseUnit, rect.y*m_baseUnit, rect.w*m_baseUnit, rect.h*m_baseUnit));
								}
								emit(registerLayer(layer_name));
							}
						}
					}*/
			}
		}
	}
}

void QLayoutScene::runDRC(QString n, QRectF rect)
{
	TechDesignRule rule;
	QString layerName;
	QGraphicsRectItem *m;
	QLayoutRectItem *w;
	QGraphicsPolygonItem *p;
	QLayoutDistanceMeasure *error_line;

	//rule = project->getDesignRule(n);

	if(rule.getName()==QString())
		return;

	if(rect.width() < rule.getMinimumWidth()) {
		qDebug() << __FUNCTION__ << "DRC error!";
		qDebug() << __FUNCTION__ << rule.getName();
		qDebug() << __FUNCTION__ << rule.getWidthMessage();
		error_line = new QLayoutDistanceMeasure(rule.getWidthMessage(),rect.bottomLeft(),rect.bottomRight());
		distance_errors.append(error_line);
		addItem(error_line);
	}
	if(rect.height() < rule.getMinimumWidth()) {
		qDebug() << __FUNCTION__ << "DRC error!";
		qDebug() << __FUNCTION__ << rule.getName();
		qDebug() << __FUNCTION__ << rule.getWidthMessage();
		error_line = new QLayoutDistanceMeasure(rule.getWidthMessage(),rect.bottomLeft(),rect.topLeft());
		distance_errors.append(error_line);
		addItem(error_line);
	}
}

void  QLayoutScene::runDRC()
{
	QString layerName;
	QGraphicsRectItem *m;
	QLayoutRectItem *w;
	QGraphicsPolygonItem *p;
	QRectF rect;

	foreach(QLayoutDistanceMeasure *t, distance_errors) {
		removeItem(t);
	}

	foreach(layerName, layer_rects.keys()) {
		foreach(w, layer_rects[layerName]) {
			rect = w->boundingRect();
			runDRC(layerName, rect);
		}
	}
}

void QLayoutScene::addPad(QString name, QString net, QString layer, qreal x, qreal y, qreal w, qreal h)
{
	QLayoutRectItem *r;
	QGraphicsSimpleTextItem *t;

	r = new QLayoutRectItem(x, y, w, h);
	t = new QGraphicsSimpleTextItem(name,r);
	t->setPos(x,y);
	t->setScale(w/t->boundingRect().width());

	r->setVisible(true);
	if(m_techData) r->setColor(m_techData->getLayerColor(layer));
	addItem(r);
	update();

	layer_rects[layer].append(r);
	emit(registerLayer(layer));
}

void QLayoutScene::addVia(QString netname, QString vianame, QPointF p)
{
	if(m_viaTemplateMap.contains(vianame)) {
		QLayoutViaItem* via = new QLayoutViaItem(m_viaTemplateMap[vianame]);
		via->setPos(p);
		via->setVisible(true);
		addItem(via);
	}
}

void QLayoutScene::addWire(QString netname, QString layer, QPointF p1, QPointF p2)
{
	QPen pen;
	QGraphicsLineItem* l = new QGraphicsLineItem(QLineF(p1,p2));
	pen = l->pen();
	pen.setWidth(1);
	pen.setCosmetic(true);
	if(m_techData) pen.setColor(m_techData->getLayerColor(layer));
	l->setPen(pen);
	addItem(l);
}

void QLayoutScene::addRectangle(QString layer, qreal x, qreal y, qreal w, qreal h)
{
	QLayoutRectItem *r;
	x*=m_gridSize;
	y*=m_gridSize;
	w*=m_gridSize;
	h*=m_gridSize;

	r = new QLayoutRectItem(x, y, w, h);

	r->setVisible(true);
	if(m_techData) r->setColor(m_techData->getLayerColor(layer));
	addItem(r);
	update();

	layer_rects[layer].append(r);
	emit(registerLayer(layer));
}

void QLayoutScene::addMacro(QString macro_name, QString instance_name, qreal x, qreal y, QString orient)
{
	QLayoutMacroItem *mi;
	qreal angle = 0;
	x*=m_gridSize;
	y*=m_gridSize;
	if(m_macroTemplateMap.contains(macro_name)) {
		mi = new QLayoutMacroItem(1,m_macroTemplateMap[macro_name]);
		mi->setInstanceName(instance_name);
		if(orient=="S") {
			angle=180;
			x+=mi->rect().width();
			y+=mi->rect().height();
		} else if(orient=="E") {
			angle=-90;
			y+=mi->rect().width();
		} else if(orient=="W") {
			angle=90;
			x+=mi->rect().height();
		} else {
			angle = 0;
		}
		mi->setRotation(angle);
		mi->setPos(x,y);
		macros.append(mi);
		cellManagerDialog->addCell(instance_name,macro_name);
		addItem(mi);
		update();
	} else {
		qDebug() << macro_name << "Macro not defined";
	}
}

void QLayoutScene::addMacro(QString macro_name, QString instance_name, qreal x, qreal y, qreal w, qreal h, QString orient)
{
	QLayoutMacroItem *mi;
	qreal angle;
	qreal tx, ty;
	x*=m_gridSize;
	y*=m_gridSize;
	w*=m_gridSize;
	h*=m_gridSize;
	if(m_macroTemplateMap.contains(macro_name)) {

		mi = new QLayoutMacroItem(1,m_macroTemplateMap[macro_name]);
		mi->setInstanceName(instance_name);

		tx = 1;
		ty = 1;

		if(orient=="N") {
			angle=0;
		} else if(orient=="FN") {
			angle=0;
			tx = -1;
		} else if(orient=="S") {
			angle=180;
		} else if(orient=="FS") {
			angle=180;
			x+=w;
			y-=h;
			ty = -1;
		} else if(orient=="E") {
			angle=90;
			x+=h;
			y-=w;
		} else if(orient=="W") {
			angle=-90;
			x-=h;
			y+=w;
		} else {
			angle = 0;
		}

		mi->setTransform(QTransform::fromScale(tx, ty));
		mi->setRotation(angle);
		mi->setPos(x,y);
		mi->setSize(w,h);
		macros.append(mi);
		cellManagerDialog->addCell(instance_name,macro_name);
		addItem(mi);

		update();
	} else {
		qDebug() << macro_name << "Macro not defined";
	}
}

QStringList QLayoutScene::getLayers()
{
	QStringList ret;
	foreach(QString s, layer_rects.keys())
		if(s!=ALL_LAYERS)
			ret.append(s);
	return ret;
}

QVector<QLayoutRectItem*> QLayoutScene::getRectangles(QString n)
{
	QVector<QLayoutRectItem*> rects;
	QLayoutRectItem *m;
	if(layer_rects.contains(n)) {
		foreach(m, layer_rects[n]) {
			if(m) rects.append(m);
		}
	}
	return rects;
}
