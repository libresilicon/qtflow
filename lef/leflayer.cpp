#include "leflayer.h"

namespace lef {
	QString LEFLayer::getName()
	{
		return m_name;
	}

	QVector<rect_t> LEFLayer::getRects()
	{
		return m_rects;
	}

	LEFLayer::LEFLayer(QString n) :
		m_name(n)
	{
	}

	void LEFLayer::addRectangle(double x, double y, double w, double h)
	{
		rect_t obj;
		obj.x = x;
		obj.y = y;
		obj.w = w;
		obj.h = h;
		m_rects.append(obj);
	}
}
