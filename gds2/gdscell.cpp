#include "gdscell.h"

GDSCell::GDSCell(QString name):
	m_name(name)
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
	return m_width;
}

qreal GDSCell::getHeight()
{
	return m_height;
}

void GDSCell::setRectangle(int x, int y, int w, int h)
{
	m_scale_x = abs(m_width/w);
	m_scale_y = abs(m_height/h);

	if(w<0) x+=w;
	if(h<0) y+=h;

	x-=(m_x/m_scale_x);
	y-=(m_y/m_scale_y);

	//x-=m_x;
	//y-=m_y;

	foreach(GDSBoundary *b, m_boundaries) {
		b->setScale(m_scale_x,m_scale_y);
		b->setOffset(x,y);
	}
}

void GDSCell::addBoundary(GDSBoundary* b)
{
	int x1,x2,y1,y2;
	if(m_boundaries.count()) {
		// update lowest x/y
		if(b->x() < m_x) m_x = b->x();
		if(b->y() < m_y) m_y = b->y();

		x1 = m_x + m_width;
		x2 = b->x() + b->width();
		y1 = m_y + m_height;
		y2 = b->y() + b->height();

		if(x2>x1) m_width = x2 - m_x;
		if(y2>y1) m_height = y2 - m_y;

	} else { // first box
		m_x = b->x();
		m_y = b->y();
		m_width = b->width();
		m_height = b->height();
	}

	m_boundaries.append(b);
}
