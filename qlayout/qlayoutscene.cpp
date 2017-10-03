#include "qlayoutscene.h"

QLayoutScene::QLayoutScene(QObject *parent) :
	QGraphicsScene(parent),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	recentRectangle(NULL)
{}

QLayoutScene::QLayoutScene(const QRectF &sceneRect, QObject *parent) :
	QGraphicsScene(sceneRect, parent),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	recentRectangle(NULL)
{}

QLayoutScene::QLayoutScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
	QGraphicsScene(x, y, width, height, parent),
	recentOperation(DRAWING_OPERATION_NONE),
	project(NULL),
	recentRectangle(NULL)
{}

void QLayoutScene::setProject(Project *p)
{
	project = p;
}

void QLayoutScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF offsetOrig;
	switch(recentOperation) {
		case DRAWING_OPERATION_RECTANGLE:
			lastOrig = event->scenePos();
			recentRectangle = new QLayoutRectItem(lastOrig.x(),lastOrig.y(),1,1);
			recentRectangle->setVisible(true);
			recentRectangle->setFlag(QGraphicsItem::ItemIsMovable, true);
			if(project) recentRectangle->setBrush(QBrush(project->colorMat(activeLayer)));
			addItem(recentRectangle);
			break;
		case DRAWING_OPERATION_DRAG:
			if(!recentRectangle) {
				lastOrig = event->scenePos();
				foreach(QLayoutRectItem *m, layer_rects[activeLayer]) {
					offsetOrig.setX(lastOrig.x()-m->x());
					offsetOrig.setY(lastOrig.y()-m->y());
					if(m->contains(offsetOrig)) {
						qDebug() << "getting here";
						recentRectangle = m;
						lastRectOrig = recentRectangle->pos();
					}
				}
			}
			break;
		default:
			qDebug() << "QLayoutScene::" << __FUNCTION__;
			break;
	}
}

void QLayoutScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qreal dx, dy;
	QPointF pt;
	QRectF r;

	pt = event->scenePos();
	switch(recentOperation) {
		case DRAWING_OPERATION_RECTANGLE:
			if(recentRectangle) {
				dx=pt.x()-lastOrig.x();
				dy=pt.y()-lastOrig.y();
				recentRectangle->setRect(lastOrig.x(),lastOrig.y(),dx,dy);;
				update();
			}
			break;
		case DRAWING_OPERATION_DRAG:
			if(recentRectangle) {
				r = recentRectangle->rect();
				dx=pt.x()-lastOrig.x();
				dy=pt.y()-lastOrig.y();
				recentRectangle->setPos(lastRectOrig.x()+dx,lastRectOrig.y()+dy);
				update();
			}
			break;
		default:
			qDebug() << "QLayoutScene::" << __FUNCTION__;
			break;
	}
}

void QLayoutScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QLayoutRectItem *m;
	QPointF pt;

	pt = event->scenePos();
	switch(recentOperation) {
		case DRAWING_OPERATION_RECTANGLE:
			layer_rects[activeLayer].append(recentRectangle);
			recentRectangle = NULL;
			break;
		case DRAWING_OPERATION_DRAG:
			recentRectangle = NULL;
			qDebug() << "QLayoutScene::" << __FUNCTION__;
			update();
			break;
		default:
			qDebug() << "QLayoutScene::" << __FUNCTION__;
			break;
	}
}

void QLayoutScene::setDrawingOperation(drawing_operations o)
{
	recentOperation = o;
}

void QLayoutScene::setActiveLayer(QString layer)
{
	activeLayer = layer;
}

void QLayoutScene::addWire(QString layer, int x, int y, int w, int h)
{
	QLayoutRectItem *r = new QLayoutRectItem(x, y, w, h);
	r->setVisible(true);
	if(project) r->setBrush(QBrush(project->colorMat(layer)));
	addItem(r);
	update();

	layer_rects[layer].append(r);
}
