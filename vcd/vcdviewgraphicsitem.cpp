#include "vcdviewgraphicsitem.h"

VcdViewGraphicsItem::VcdViewGraphicsItem(vcd::Var var, QGraphicsItem *parent) :
	QGraphicsItem(parent),
	m_name(QString::fromStdString(var.name())),
	m_height(20),
	m_lastTime(10),
	m_firstTime(0),
	m_firstTimeSet(false)
{
	QGraphicsSimpleTextItem *text;
	text = new QGraphicsSimpleTextItem(m_name, this);
	//text->setPos(recentZeroTime+space, drawingIndex*height+space);
	text->setBrush(QColor(Qt::white));
	text->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
}

void VcdViewGraphicsItem::addTime(qreal time)
{
	if(m_firstTimeSet) {
		if(time > m_lastTime) m_lastTime = time;
		if(time < m_firstTime) m_firstTime = time;
	} else {
		m_firstTime = time;
		m_lastTime = time;
		m_firstTimeSet = true;
	}
}

QRectF VcdViewGraphicsItem::boundingRect() const
{
	return QRect(m_firstTime,0,m_lastTime-m_firstTime,m_height);
}

void VcdViewGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

qreal VcdViewGraphicsItem::height()
{
	return m_height;
}

QPointF VcdViewGraphicsItem::bottomLeft()
{
	QPointF p = pos();
	QRectF r = boundingRect();
	r.setX(p.x());
	r.setY(p.y());

	return r.bottomLeft();
}

bool VcdViewGraphicsItem::contains(const QPointF &point) const
{
	QPointF p = pos();
	QRectF r = boundingRect();
	r.setX(0);
	r.setY(p.y());

	return r.contains(point);
}
