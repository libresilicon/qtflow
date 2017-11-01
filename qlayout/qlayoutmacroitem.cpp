#include "qlayoutmacroitem.h"

QLayoutMacroItem::QLayoutMacroItem(QLayoutMacroItem *parent) :
	QGraphicsRectItem(0),
	m_dragged(false),
	m_instanceNameLabel(NULL)
{
	QGraphicsRectItem* r;
	QRectF rect;
	QPen pen(Qt::black);
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	pen.setWidth(30);
	setPen(pen);
	if(parent) {
		rect = parent->rect();
		setRect(rect);
	}

	setInstanceName(parent->m_instanceName);
	setMacroName(parent->m_macroName);

	foreach(QString layer, parent->m_layerList.keys()) {
		foreach(QGraphicsRectItem* m, parent->m_layerList[layer]) {
			if(m) {
				addRectangle(layer,m->brush(),m->rect());
			}
		}
	}
}

void QLayoutMacroItem::addRectangle(QString layer, QBrush brush, QRectF rect)
{
	if(layer==QString())
		return;

	QGraphicsRectItem* r = new QGraphicsRectItem(rect,this);
	r->setVisible(true);
	r->setBrush(brush);
	r->setOpacity(0.75);
	m_layerList[layer].append(r);
}

void QLayoutMacroItem::addPolygon(QString layer, QBrush brush, QPolygonF poly)
{
	addRectangle(layer, brush, poly.boundingRect());
}

QLayoutMacroItem::QLayoutMacroItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent),
	   m_dragged(false)
{

}

void QLayoutMacroItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	m_dragged = true;
	QGraphicsRectItem::mouseMoveEvent(event);
}

void QLayoutMacroItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

QString QLayoutMacroItem::getMacroName()
{
	return m_macroName;
}

void QLayoutMacroItem::setInstanceName(QString n)
{
	m_instanceName = n;

	m_instanceNameLabel = new QGraphicsSimpleTextItem(m_instanceName,this);
	m_instanceNameLabel->setVisible(true);
}

void QLayoutMacroItem::setMacroName(QString n)
{
	m_macroName = n;

	m_macroNameLabel = new QGraphicsSimpleTextItem(m_macroName,this);
	m_macroNameLabel->setVisible(true);
}

void QLayoutMacroItem::setWidth(qreal w)
{
	m_width = w;
}

void QLayoutMacroItem::setHeight(qreal h)
{
	m_height = h;
}

QString QLayoutMacroItem::getInstanceName()
{
	return m_instanceName;
}

void QLayoutMacroItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *markAction = menu.addAction("Mark");
	QAction *selectedAction = menu.exec(event->screenPos());
}
