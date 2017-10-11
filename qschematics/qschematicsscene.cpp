#include "qschematicsscene.h"

QSchematicsScene::QSchematicsScene(QObject *parent) :
	QGraphicsScene(parent),
	project(NULL)
{
}

QSchematicsScene::QSchematicsScene(const QRectF &sceneRect, QObject *parent) :
	QGraphicsScene(sceneRect, parent),
	project(NULL)
{
}

QSchematicsScene::QSchematicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
	QGraphicsScene(x, y, width, height, parent),
	project(NULL)
{
}

void QSchematicsScene::addWire(QString type, qreal x1, qreal y1, qreal x2, qreal y2)
{
	QSchematicsWire *w = new QSchematicsWire(type, x1, y1, x2, y2);
	wires.append(w);
	addItem(w);
}

void QSchematicsScene::addPart(QString name, QString type, qreal x, qreal y)
{
	//QSchematicsParametricPart *p = new QSchematicsParametricPart(name, x, y);
	//parts.append(p);
	//addItem(p);
}

void QSchematicsScene::setProject(Project *p)
{
	project = p;
}

