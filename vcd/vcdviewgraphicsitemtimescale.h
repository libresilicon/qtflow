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


};

#endif // VCDVIEWGRAPHICSITEMTIMESCALE_H
