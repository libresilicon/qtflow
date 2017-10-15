#ifndef VCDVIEWGRAPHICSITEMSIGNAL_H
#define VCDVIEWGRAPHICSITEMSIGNAL_H

#include <QGraphicsItem>
#include <QRect>
#include <QPen>

#include "vcd_reader/vcd_data.hpp"

class VcdViewGraphicsItemSignal : public QGraphicsItem
{
public:
	explicit VcdViewGraphicsItemSignal(vcd::Var var, vcd::TimeValues values, QGraphicsItem *parent = nullptr);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

signals:

public slots:

private:
	// bounding rect:
	qreal m_heigth;
	qreal m_width;

	QString m_name;
};

#endif // VCDVIEWGRAPHICSITEMSIGNAL_H
