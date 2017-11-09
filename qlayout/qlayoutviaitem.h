#ifndef QLAYOUTVIAITEM_H
#define QLAYOUTVIAITEM_H

#include <QGraphicsRectItem>
#include <QPen>
#include <QDebug>

class QLayoutViaItem : public QGraphicsRectItem
{
public:
	explicit QLayoutViaItem(QLayoutViaItem *parent);
	explicit QLayoutViaItem(qreal x, qreal y, qreal w, qreal h);

	void addRectangle(QString layer, QBrush brush, QRectF rect);

private:
	bool m_dragged;
	QString m_viaName;
	qreal m_width;
	qreal m_height;

	QMap<QString,QVector<QGraphicsRectItem*>> m_layerList;
};

#endif // QLAYOUTVIAITEM_H
