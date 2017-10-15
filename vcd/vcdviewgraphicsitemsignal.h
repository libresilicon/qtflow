#ifndef VCDVIEWGRAPHICSITEMSIGNAL_H
#define VCDVIEWGRAPHICSITEMSIGNAL_H

#include <QGraphicsItem>
#include <QRect>

class VcdViewGraphicsItemSignal : public QGraphicsItem
{
public:
	explicit VcdViewGraphicsItemSignal(QGraphicsItem *parent = nullptr);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

signals:

public slots:

private:
	// bounding rect:
	void setBoundingRect();
	qreal m_xmin;
	qreal m_xmax;
	qreal m_ymin;
	qreal m_ymax;

};

#endif // VCDVIEWGRAPHICSITEMSIGNAL_H
