#include "qschematicswire.h"

QSchematicsWire::QSchematicsWire(QString type, qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) :
	QGraphicsLineItem(x1,y1,x2,y2,parent)
{
	QPen pen = this->pen();
	pen.setWidth(5);
	setPen(pen);
	setVisible(true);
}
