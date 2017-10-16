#ifndef VCDVIEWGRAPHICSITEM_H
#define VCDVIEWGRAPHICSITEM_H

#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QRect>
#include <QPen>

#include "vcd_reader/vcd_data.hpp"

class VcdViewGraphicsItem : public QGraphicsItem
{
public:
	explicit VcdViewGraphicsItem(vcd::Var var, QGraphicsItem *parent = nullptr);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

	qreal height();
	qreal width();

	QPointF bottomLeft();
	bool contains(const QPointF &point) const;

	QString getLongName();

protected:
	void addTime(qreal time);

signals:

public slots:

private:
	// bounding rect:
	qreal m_height;
	qreal m_width;

	qreal m_lastTime;
	qreal m_firstTime;
	bool m_firstTimeSet;

	QString m_name;
	QString m_long_name;
};

#endif // VCDVIEWGRAPHICSITEM_H
