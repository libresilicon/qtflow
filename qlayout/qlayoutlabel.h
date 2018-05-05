#ifndef QLAYOUTLABEL_H
#define QLAYOUTLABEL_H

#include <QGraphicsItem>

class QLayoutLabel : public QGraphicsItem
{
public:
	QLayoutLabel();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
};

#endif // QLAYOUTLABEL_H
