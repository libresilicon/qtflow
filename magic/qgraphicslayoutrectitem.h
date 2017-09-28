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

public slots:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	bool m_dragged;
};

#endif // QGRAPHICSLAYOUTRECTITEM_H
