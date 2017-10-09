#include "qschematicsscene.h"

QSchematicsScene::QSchematicsScene(QObject *parent) :
	QGraphicsScene(parent)
{
}

QSchematicsScene::QSchematicsScene(const QRectF &sceneRect, QObject *parent) :
	QGraphicsScene(sceneRect, parent)
{
}

QSchematicsScene::QSchematicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
	QGraphicsScene(x, y, width, height, parent)
{
}

void QSchematicsScene::addWire(QString name, qreal x1, qreal y1, qreal x2, qreal y2)
{
	QPen pen;
	QGraphicsLineItem *w = new QGraphicsLineItem(x1,y1,x2,y2);
	pen = w->pen();
	pen.setWidth(5);
	w->setPen(pen);
	w->setVisible(true);
	addItem(w);
}
