#ifndef QSCHEMATICSSCENE_H
#define QSCHEMATICSSCENE_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QRubberBand>
#include <QGraphicsProxyWidget>
#include <QShortcut>

#include "project.h"

class QSchematicsScene : public QGraphicsScene
{
public:
	QSchematicsScene(QObject *parent = Q_NULLPTR);
	QSchematicsScene(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
	QSchematicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);

	void addWire(QString name, QPointF p1, QPointF p2);

};

#endif // QSCHEMATICSSCENE_H
