#include "qschematicsscene.h"

QSchematicsScene::QSchematicsScene(QObject *parent) :
	QGraphicsScene(parent),
	m_sheet(NULL)
{
}

QSchematicsScene::QSchematicsScene(const QRectF &sceneRect, QObject *parent) :
	QGraphicsScene(sceneRect, parent),
	m_sheet(NULL)
{
}

QSchematicsScene::QSchematicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
	QGraphicsScene(x, y, width, height, parent),
	m_sheet(NULL)
{
}

QGraphicsRectItem* QSchematicsScene::setSheet(int w, int h)
{
	QPen pen;
	m_sheet = new QGraphicsRectItem(0,0,w,h);
	m_sheet->setVisible(true);
	pen = m_sheet->pen();
	pen.setWidth(10);
	m_sheet->setPen(pen);
	addItem(m_sheet);
	update();
	return m_sheet;
}

void QSchematicsScene::addWire(QString type, qreal x1, qreal y1, qreal x2, qreal y2)
{
	QSchematicsWire *w = new QSchematicsWire(type, x1, y1, x2, y2, m_sheet);
	m_wires.append(w);
}

void QSchematicsScene::addPart(QString type, QString id, int x, int y)
{
	QSchematicsPart* part;
	symbol::SchematicsSymbol* partsymbol;
	/*if(m_project) if(m_project->isDefinedPart(type)) {
		partsymbol = m_project->getSchematicsPart(type);
		if(partsymbol) {
			part = new QSchematicsPart(partsymbol, id, x, y, m_sheet);
			m_parts.append(part);
		}
	}*/
}

void QSchematicsScene::addPart(QString type, int x, int y)
{
	QSchematicsPart* part;
	symbol::SchematicsSymbol* partsymbol;
	/*if(m_project) if(m_project->isDefinedPart(type)) {
		partsymbol = m_project->getSchematicsPart(type);
		if(partsymbol) {
			part = new QSchematicsPart(partsymbol, x, y, m_sheet);
			m_parts.append(part);
		}
	}*/
}

void QSchematicsScene::setDrawingOperation(schematics_operations o)
{
	m_recentOperation = o;
}

void QSchematicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_lastOrig = event->scenePos();

	switch(m_recentOperation) {
		case SCHEMATICS_OPERATION_DRAG:
			if(m_dragging) return;
			m_dragging = true;
			foreach(QSchematicsPart *p, m_parts) {
				if(p->contains(m_lastOrig)||p->isSelected()) {
					if(!p->isSelected()) {
						foreach(QSchematicsPart *m, m_parts) {
							m->setSelected(false);
						}
					}
					p->setSelected(true);
					p->setDragMode(true);
					p->setCursor(QCursor(Qt::ClosedHandCursor));
				}
			}
			break;
	}
}

void QSchematicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qreal dx, dy;
	QPointF pt;

	//pt = snapGrid(event->scenePos());
	pt = event->scenePos();

	switch(m_recentOperation) {
		case SCHEMATICS_OPERATION_DRAG:
			if(!m_dragging) return;
			foreach(QSchematicsPart *m, m_parts) {
				if(m->isSelected()) {
					dx=pt.x()-m_lastOrig.x();
					dy=pt.y()-m_lastOrig.y();
					m->updateMovingOffset(dx,dy);
				}
			}
			update();
			break;
	}
}

void QSchematicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	switch(m_recentOperation) {
		case SCHEMATICS_OPERATION_DRAG:
			foreach(QSchematicsPart *m, m_parts) {
				m->setCursor(QCursor(Qt::ArrowCursor));
				m->setDragMode(false);
				m->setSelected(false);
				m_dragging = false;
			}
			update();
			break;
	}
}
