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

#include "project.h"

#include "lef/lefdata.h"

#include "qschematicswire.h"
#include "qschematicspart.h"

class QSchematicsScene : public QGraphicsScene
{
public:
	QSchematicsScene(QObject *parent = Q_NULLPTR);
	QSchematicsScene(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
	QSchematicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);

	void addWire(QString type, qreal x1, qreal y1, qreal x2, qreal y2);
	void addPart(QString name, QString type, qreal x, qreal y);

	void setProject(Project *p);
	void setLEF(lef::LEFData *d);

private:
	QVector<QSchematicsWire*> wires;
	QVector<QSchematicsPart*> parts;

	Project *project;
	lef::LEFData *lefdata;
};

#endif // QSCHEMATICSSCENE_H
