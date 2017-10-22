#ifndef QSCHEMATICSPART_H
#define QSCHEMATICSPART_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

#include "symbol_reader/schematicssymbol.h"

class QSchematicsPart : public QObject, public QGraphicsItem
{
	Q_OBJECT

public:
	QSchematicsPart(symbol::SchematicsSymbol*obj, QString name, int x, int y, QGraphicsItem *parent = Q_NULLPTR);
	QSchematicsPart(symbol::SchematicsSymbol*obj, int x, int y, QGraphicsItem *parent = Q_NULLPTR);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

	bool contains(const QPointF &point) const Q_DECL_OVERRIDE;
	bool setDragMode(bool m);
	void updateMovingOffset(qreal dx, qreal dy);

	qreal width();
	qreal height();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	QString m_name;

	QVector<QSchematicsPin*> m_pins;
	QGraphicsSimpleTextItem *m_partLabel;
	QGraphicsSimpleTextItem *m_typeLabel;
	QGraphicsRectItem *m_partFrame;
	QRectF m_externalRect;
	QRectF m_boundingRect;

	QPointF m_lastOrig;
	bool m_dragMode;

	// bounding rect:
	void setBoundingRect();
	qreal m_xmin;
	qreal m_xmax;
	qreal m_ymin;
	qreal m_ymax;
};

#endif // QSCHEMATICSPART_H
