#ifndef VCDVIEWGRAPHICSITEMBUS_H
#define VCDVIEWGRAPHICSITEMBUS_H

#include <QGraphicsItem>
#include <QRect>

#include "vcd_reader/vcd_data.hpp"

class VcdViewGraphicsItemBus : public QGraphicsItem
{
public:
	explicit VcdViewGraphicsItemBus(vcd::Var var, vcd::TimeBusValues values, QGraphicsItem *parent = nullptr);

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

#endif // VCDVIEWGRAPHICSITEMBUS_H
