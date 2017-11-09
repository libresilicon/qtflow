#include "qlayoutviaitem.h"

QLayoutViaItem::QLayoutViaItem(QLayoutViaItem *parent) : // cloning the template object and it's children
	QGraphicsRectItem(0),
	m_dragged(false),
	m_width(1),
	m_height(1)
{
	QRectF rect;
	QPen pen(Qt::black);
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	//pen.setWidth(30);
	//setPen(pen);
	//setBrush(QBrush(Qt::blue));

	if(parent) {
		rect = parent->rect();
		//setRect(rect);
		m_width = rect.width();
		m_height = rect.height();

		foreach(QString layer, parent->m_layerList.keys()) {
			foreach(QGraphicsRectItem* m, parent->m_layerList[layer]) {
				if(m) {
					addRectangle(layer,m->brush(),m->rect());
				}
			}
		}
	}
}

QLayoutViaItem::QLayoutViaItem(qreal x, qreal y, qreal w, qreal h) :
	QGraphicsRectItem(x,y,w,h),
	m_dragged(false)
{
	//setRect(x,y,w,h);
}

void QLayoutViaItem::addRectangle(QString layer, QBrush brush, QRectF rect)
{
	if(layer==QString())
		return;

	QGraphicsRectItem* r = new QGraphicsRectItem(rect,this);
	r->setVisible(true);
	r->setBrush(brush);
	r->setOpacity(0.75);
	m_layerList[layer].append(r);
}
