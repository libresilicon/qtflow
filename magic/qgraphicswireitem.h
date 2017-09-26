#ifndef QGRAPHICSWIREITEM_H
#define QGRAPHICSWIREITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>

class QGraphicsWireItem : public QGraphicsRectItem
{
public:
	explicit QGraphicsWireItem(QGraphicsItem *parent = Q_NULLPTR);
	explicit QGraphicsWireItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:

public slots:

private:
	bool m_dragged;
};

#endif // QGRAPHICSWIREITEM_H
