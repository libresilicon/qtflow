#include "lefpin.h"

namespace lef {
	LEFPort* LEFPin::getPort()
	{
		return port;
	}

	LEFPin::LEFPin(QString n) :
		port(new LEFPort()),
		m_x(INT_MAX),
		m_y(INT_MAX),
		m_w(0),
		m_h(0)
	{
		name = n;
	}

	void LEFPin::setBoundingBox(double x, double y, double w, double h)
	{
		if(x<m_x) m_x = x;
		if(y<m_y) m_y = y;
		if(w>m_w) m_w = w;
		if(h>m_h) m_h = h;
	}

	QPointF LEFPin::getCenter()
	{
		qreal xp, yp;
		xp = m_x+m_w/2;
		yp = m_y+m_h/2;
		return QPointF(xp,yp);
	}

	QString LEFPin::getName()
	{
		return name;
	}

	QVector<LEFLayer*> LEFPin::getPortLayers()
	{
		return port->getLayers();
	}
}
