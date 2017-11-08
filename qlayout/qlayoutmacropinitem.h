#ifndef QLAYOUTMACROPINITEM_H
#define QLAYOUTMACROPINITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QString>
#include <QBrush>
#include <QRectF>
#include <QPen>

class QLayoutMacroPinItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
public:
	explicit QLayoutMacroPinItem(QLayoutMacroPinItem *orig = Q_NULLPTR, QGraphicsItem *parent = Q_NULLPTR);
	explicit QLayoutMacroPinItem(QGraphicsItem *parent = Q_NULLPTR);

	void addRectangle(QString layer, QBrush brush, QRectF rect);
	void setName(QString s);

signals:

public slots:
private:
	QString m_name;
	QGraphicsSimpleTextItem* m_label;
	QMap<QString,QVector<QGraphicsRectItem*>> m_layerList;

	qreal m_x1;
	qreal m_x2;
	qreal m_y1;
	qreal m_y2;
};

#endif // QLAYOUTMACROPINITEM_H
