#ifndef LEFPIN_H
#define LEFPIN_H

#include <QString>
#include <QPointF>

#include "lefport.h"

namespace lef {
class LEFPin {
public:
	LEFPin(QString);
	void scalePin(qreal, qreal);
	void setBoundingBox(double x, double y, double w, double h);

	QString getName();
	LEFPort *getPort();
	QVector<LEFLayer*> getPortLayers();
	QPointF getCenter();

private:
	QString name;
	LEFPort *port;

	// bounding box:
	double m_x;
	double m_y;
	double m_w;
	double m_h;
};
}


#endif // LEFPIN_H
