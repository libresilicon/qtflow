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

#include "qschematicswire.h"
#include "qschematicspart.h"

class QSchematicsScene : public QGraphicsScene
{
public:
	QSchematicsScene(QObject *parent = Q_NULLPTR);
	QSchematicsScene(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
	QSchematicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);

	void addWire(QString type, qreal x1, qreal y1, qreal x2, qreal y2);
	void addPart(QString type, QString id, int x, int y);
	void addPart(QString type, int x, int y);

	void setProject(Project *p);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	QVector<QSchematicsWire*> m_wires;
	QVector<QSchematicsPart*> m_parts;

	Project *m_project;

	QPointF m_lastOrig;
	bool m_dragging;
};

#endif // QSCHEMATICSSCENE_H
