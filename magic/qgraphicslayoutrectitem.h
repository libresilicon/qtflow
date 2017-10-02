#ifndef QGRAPHICSLAYOUTRECTITEM_H
#define QGRAPHICSLAYOUTRECTITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QGraphicsScene>
#include <QDebug>

class QGraphicsLayoutRectItem : public QGraphicsRectItem
{
public:
	explicit QGraphicsLayoutRectItem(QGraphicsItem *parent = Q_NULLPTR);
	explicit QGraphicsLayoutRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);

protected:

signals:

private:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
};

#endif // QGRAPHICSLAYOUTRECTITEM_H
