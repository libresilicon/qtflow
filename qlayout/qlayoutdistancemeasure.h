#ifndef QLAYOUTDISTANCEMEASURE_H
#define QLAYOUTDISTANCEMEASURE_H

#include <QGraphicsItem>

class QLayoutDistanceMeasure : public QObject, public QGraphicsItem
{
	Q_OBJECT

public:
	QLayoutDistanceMeasure(QString label, QPointF start, QPointF end, QGraphicsItem *parent = Q_NULLPTR);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

private:
	QGraphicsSimpleTextItem *m_label;
	QGraphicsLineItem *m_mainLine;

};

#endif // QLAYOUTDISTANCEMEASURE_H
