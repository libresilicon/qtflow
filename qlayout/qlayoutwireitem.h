#ifndef QLAYOUTWIREITEM_H
#define QLAYOUTWIREITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>

class QLayoutWireItem : public QGraphicsRectItem
{
public:
	explicit QLayoutWireItem(QGraphicsItem *parent = Q_NULLPTR);
	explicit QLayoutWireItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:

public slots:

private:
	bool m_dragged;
};

#endif // QLAYOUTWIREITEM_H
