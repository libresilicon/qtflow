#include "lefpin.h"

namespace lef {
	LEFPort* LEFPin::getPort()
	{
		return port;
	}

	LEFPin::LEFPin(QString n) :
		port(new LEFPort()),
		m_x1(INT_MAX),
		m_y1(INT_MAX),
		m_x2(0),
		m_y2(0),
		m_direction(PIN_INOUT)
	{
		name = n;
	}

	void LEFPin::setDirection(QString s)
	{
		if(s=="INPUT")
			m_direction = PIN_INPUT;
		if(s=="INOUT")
			m_direction = PIN_INOUT;
		if(s=="OUTPUT")
			m_direction = PIN_OUTPUT;
	}

	pin_direction LEFPin::getDirection()
	{
		return m_direction;
	}

	void LEFPin::setBoundingBox(double x1, double y1, double x2, double y2)
	{
		if(x1<m_x1) m_x1 = x1;
		if(y1<m_y1) m_y1 = y1;
		if(x2>m_x2) m_x2 = x2;
		if(y2>m_y2) m_y2 = y2;
	}

	QPointF LEFPin::getCenter()
	{
		qreal xp, yp;
		xp = m_x1+(m_x2-m_x1)/2;
		yp = m_y1+(m_y2-m_y1)/2;
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
