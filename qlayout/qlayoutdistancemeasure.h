#ifndef QLAYOUTDISTANCEMEASURE_H
#define QLAYOUTDISTANCEMEASURE_H

#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QDebug>

class QLayoutDistanceMeasure : public QGraphicsItem
{
public:
	QLayoutDistanceMeasure(QString s, QPointF p1, QPointF p2, QGraphicsItem *parent = Q_NULLPTR);
	QLayoutDistanceMeasure(QGraphicsItem *parent = Q_NULLPTR);
	void updateLabel(QString s);
	void setLine(qreal x1, qreal y1, qreal x2, qreal y2);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

private:
	QGraphicsSimpleTextItem *m_label;
	QGraphicsLineItem *m_mainLine;
	QPointF m_p1;
	QPointF m_p2;
};

#endif // QLAYOUTDISTANCEMEASURE_H
