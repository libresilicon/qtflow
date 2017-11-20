#include "qlayoutmacropinitem.h"

QLayoutMacroPinItem::QLayoutMacroPinItem(qreal scale, QLayoutMacroPinItem *orig, QGraphicsItem *parent) :
	QGraphicsRectItem(parent)
{
	QRectF rect;
	m_x1 = 0;
	m_x2 = 0;
	m_y1 = 0;
	m_y2 = 0;
	foreach(QString layer, orig->m_layerList.keys()) {
		foreach(QGraphicsRectItem* r, orig->m_layerList[layer]) {
			rect = r->rect();
			rect = QRectF(rect.x()*scale,rect.y()*scale,rect.width()*scale,rect.height()*scale);
			addRectangle(layer,r->brush(),rect);
			if(m_x1>rect.x()) m_x1 = rect.x();
			if(m_y1>rect.y()) m_y1 = rect.y();
			if(m_x2<(rect.x()+rect.width())) m_x2 = rect.x()+rect.width();
			if(m_y2<(rect.y()+rect.height())) m_y2 = rect.y()+rect.height();
		}
	}
	setRect(m_x1,m_y1,m_x2-m_x1,m_y2-m_y1);
	setName(orig->m_name);
}

QLayoutMacroPinItem::QLayoutMacroPinItem(QGraphicsItem *parent) :
	QGraphicsRectItem(parent)
{
}

void QLayoutMacroPinItem::addRectangle(QString layer, QBrush brush, QRectF rect)
{
	if(layer==QString())
		return;

	QPen p;
	QGraphicsRectItem* r = new QGraphicsRectItem(rect,this);
	r->setVisible(true);
	r->setBrush(brush);
	r->setOpacity(0.75);
	p = r->pen();
	p.setCosmetic(true);
	r->setPen(p);
	m_layerList[layer].append(r);
}

void QLayoutMacroPinItem::setName(QString s)
{
	qreal scale;
	qreal w,h;
	w = m_x2-m_x1;
	h = m_y2-m_y1;

	m_name = s;
	m_label = new QGraphicsSimpleTextItem(s,this);

	//scale = (w<h)?w/(m_label->boundingRect().width()):h/(m_label->boundingRect().height());
	//scale = 10000/w;

	//m_label->setScale(scale);
	m_label->setPos(m_x1,m_y1);
}
