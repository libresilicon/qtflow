#include "gdscell.h"

GDSCell::GDSCell(QString name):
	m_name(name),
	m_scale(1)
{
}

QString GDSCell::getName()
{
	return m_name;
}


QVector<GDSBoundary*> GDSCell::getBoundaries()
{
	return m_boundaries;
}

qreal GDSCell::getWidth()
{
	return m_x2-m_x1;
}

qreal GDSCell::getHeight()
{
	return m_y2-m_y1;
}

void GDSCell::setRectangle(qreal x, qreal y, qreal w, qreal h)
{
	qreal width = m_x2-m_x1;
	qreal height = m_y2-m_y1;
	qreal scale_x = (qFabs(w)>0)?qFabs(width/w):1;
	qreal scale_y = (qFabs(h)>0)?qFabs(height/h):1;

	m_scale = scale_x;
	if((m_scale*height)>h) m_scale = scale_y;

	if(w<0) x+=w;
	if(h<0) y+=h;

	x-=(m_x1/m_scale);
	y-=(m_y1/m_scale);

	foreach(GDSBoundary *b, m_boundaries) {
		b->setScale(m_scale,m_scale);
		b->setOffset(x,y);
	}
}

void GDSCell::addBoundary(GDSBoundary* b)
{
	int x1,x2,y1,y2;
	x1 = b->x();
	x2 = b->x()+b->width();
	y1 = b->y();
	y2 = b->y()+b->height();
	if(m_boundaries.count()) {
		// update lowest x/y
		if(x1<m_x1) m_x1 = x1;
		if(y1<m_y1) m_y1 = y1;
		if(x2>m_x2) m_x2 = x2;
		if(y2>m_y2) m_y2 = y2;
	} else { // first box
		m_x1 = x1;
		m_y1 = y1;
		m_x2 = x2;
		m_y2 = y2;
	}
	m_boundaries.append(b);
}
