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

void QSchematicsScene::addWire(QString name, QPointF p1, QPointF p2)
{
	qDebug() << "Adding wire: " << name;
	qDebug() << p1;
	qDebug() << p2;

	QGraphicsLineItem *w = new QGraphicsLineItem(p1.x(),p1.y(),p2.x(),p2.y());
	w->setVisible(true);
	addItem(w);
}
