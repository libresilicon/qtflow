#ifndef VCDVIEWGRAPHICSITEMTIMESCALE_H
#define VCDVIEWGRAPHICSITEMTIMESCALE_H

#include <QGraphicsItem>
#include <QRect>

class VcdViewGraphicsItemTimeScale : public QGraphicsItem
{
public:
	explicit VcdViewGraphicsItemTimeScale(QGraphicsItem *parent = nullptr);

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

#endif // VCDVIEWGRAPHICSITEMTIMESCALE_H
