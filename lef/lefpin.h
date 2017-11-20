#ifndef LEFPIN_H
#define LEFPIN_H

#include <QString>
#include <QPointF>
#include <QRectF>

#include "lefport.h"

namespace lef {
enum pin_direction {
	PIN_INPUT,
	PIN_OUTPUT,
	PIN_INOUT
};
class LEFPin {
public:
	LEFPin(QString);
	void setBoundingBox(double x1, double y1, double x2, double y2);
	void setDirection(QString s);
	QRectF getBoundingBox();

	QString getName();
	LEFPort *getPort();
	QVector<LEFLayer*> getPortLayers();
	QPointF getCenter();
	pin_direction getDirection();
	qreal width();
	qreal height();

private:
	QString name;
	LEFPort *port;
	pin_direction m_direction;

	// bounding box:
	qreal m_x1;
	qreal m_y1;
	qreal m_x2;
	qreal m_y2;
	bool m_bbox_set;
};
}


#endif // LEFPIN_H
