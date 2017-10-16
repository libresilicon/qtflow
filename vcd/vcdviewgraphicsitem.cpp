#include "vcdviewgraphicsitem.h"

VcdViewGraphicsItem::VcdViewGraphicsItem(vcd::Var var, QGraphicsItem *parent) :
	QGraphicsItem(parent),
	m_name(QString::fromStdString(var.name())),
	m_height(20),
	m_lastTime(10),
	m_firstTime(0),
	m_firstTimeSet(false)
{
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

void VcdViewGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

qreal VcdViewGraphicsItem::height()
{
	return m_height;
}
