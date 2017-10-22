#ifndef QSCHEMATICSWIRE_H
#define QSCHEMATICSWIRE_H

#include <QGraphicsLineItem>
#include <QPen>

class QSchematicsWire : public QGraphicsLineItem
{
public:
	QSchematicsWire(QString type, qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent);
};

#endif // QSCHEMATICSWIRE_H
