#ifndef VCDVIEWGRAPHICSITEMBUSSUBSIGNAL_H
#define VCDVIEWGRAPHICSITEMBUSSUBSIGNAL_H

#include <QGraphicsItem>
#include <QRect>

#include "vcd_reader/vcd_data.hpp"

class VcdViewGraphicsItemBusSubSignal : public QGraphicsItem
{
public:
	explicit VcdViewGraphicsItemBusSubSignal(vcd::Var var, vcd::TimeBusValues values, QGraphicsItem *parent = nullptr);

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

#endif // VCDVIEWGRAPHICSITEMBUSSUBSIGNAL_H
