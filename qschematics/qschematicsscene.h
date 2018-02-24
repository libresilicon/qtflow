#ifndef QSCHEMATICSSCENE_H
#define QSCHEMATICSSCENE_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QRubberBand>
#include <QGraphicsProxyWidget>
#include <QShortcut>
#include <QPainter>

#include "qschematicswire.h"
#include "qschematicspart.h"

enum schematics_operations {
	SCHEMATICS_OPERATION_NONE,
	SCHEMATICS_OPERATION_SELECT,
	SCHEMATICS_OPERATION_DRAG
};

class QSchematicsScene : public QGraphicsScene
{
public:
	QSchematicsScene(QObject *parent = Q_NULLPTR);
	QSchematicsScene(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
	QSchematicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);

	void addWire(QString type, qreal x1, qreal y1, qreal x2, qreal y2);
	void addPart(QString type, QString id, int x, int y);
	void addPart(QString type, int x, int y);
	QGraphicsRectItem *setSheet(int w, int h);

	void setDrawingOperation(schematics_operations o);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	QVector<QSchematicsWire*> m_wires;
	QVector<QSchematicsPart*> m_parts;
	QGraphicsRectItem *m_sheet;

	QPointF m_lastOrig;
	bool m_dragging;
	schematics_operations m_recentOperation;
};

#endif // QSCHEMATICSSCENE_H
