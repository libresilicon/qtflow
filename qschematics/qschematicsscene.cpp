#include "qschematicsscene.h"

QSchematicsScene::QSchematicsScene(QObject *parent) :
	QGraphicsScene(parent),
	m_project(NULL)
{
}

QSchematicsScene::QSchematicsScene(const QRectF &sceneRect, QObject *parent) :
	QGraphicsScene(sceneRect, parent),
	m_project(NULL)
{
}

QSchematicsScene::QSchematicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
	QGraphicsScene(x, y, width, height, parent),
	m_project(NULL)
{
}

void QSchematicsScene::addWire(QString type, qreal x1, qreal y1, qreal x2, qreal y2)
{
	QSchematicsWire *w = new QSchematicsWire(type, x1, y1, x2, y2);
	m_wires.append(w);
	addItem(w);
}

void QSchematicsScene::addPart(QString type, QString id, int x, int y)
{
	QSchematicsPart* part;
	symbol::SchematicsSymbol* partsymbol;
	if(m_project) if(m_project->isDefinedPart(type)) {
		partsymbol = m_project->getSchematicsPart(type);
		if(partsymbol) {
			part = new QSchematicsPart(partsymbol, id, x, y);
			addItem(part);
			m_parts.append(part);
		}
	}
}

void QSchematicsScene::addPart(QString type, int x, int y)
{
	QSchematicsPart* part;
	symbol::SchematicsSymbol* partsymbol;
	if(m_project) if(m_project->isDefinedPart(type)) {
		partsymbol = m_project->getSchematicsPart(type);
		if(partsymbol) {
			part = new QSchematicsPart(partsymbol, x, y);
			addItem(part);
			m_parts.append(part);
		}
	}
}

void QSchematicsScene::setProject(Project *p)
{
	m_project = p;
}

void QSchematicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_lastOrig = event->scenePos();
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
}

void QSchematicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qreal dx, dy;
	QPointF pt;

	//pt = snapGrid(event->scenePos());
	pt = event->scenePos();

	if(!m_dragging) return;
	foreach(QSchematicsPart *m, m_parts) {
		if(m->isSelected()) {
			dx=pt.x()-m_lastOrig.x();
			dy=pt.y()-m_lastOrig.y();
			m->updateMovingOffset(dx,dy);
		}
	}
	update();
}

void QSchematicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	foreach(QSchematicsPart *m, m_parts) {
		m->setCursor(QCursor(Qt::ArrowCursor));
		m->setDragMode(false);
		m_dragging = false;
	}
	update();
}
