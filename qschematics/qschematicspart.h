#ifndef QSCHEMATICSPART_H
#define QSCHEMATICSPART_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

#include "symbol_reader/schematicssymbol.h"

class QSchematicsPart : public QGraphicsItem
{
public:
	QSchematicsPart(symbol::SchematicsSymbol*obj, QString name, int x, int y, QGraphicsItem *parent = Q_NULLPTR);
	QSchematicsPart(symbol::SchematicsSymbol*obj, int x, int y, QGraphicsItem *parent = Q_NULLPTR);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

private:
	QString m_name;

	QVector<QSchematicsPin*> m_pins;
	QGraphicsSimpleTextItem *m_partLabel;
	QGraphicsSimpleTextItem *m_typeLabel;
	QGraphicsRectItem* m_externalRect;

	// bounding rect:
	void setBoundingRect();
	qreal m_xmin;
	qreal m_xmax;
	qreal m_ymin;
	qreal m_ymax;
};

#endif // QSCHEMATICSPART_H
