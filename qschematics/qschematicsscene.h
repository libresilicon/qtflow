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

	void addWire(QString name, qreal x1, qreal y1, qreal x2, qreal y2);

};

#endif // QSCHEMATICSSCENE_H
