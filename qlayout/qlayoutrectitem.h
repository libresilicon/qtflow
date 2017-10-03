#ifndef QLAYOUTRECTITEM_H
#define QLAYOUTRECTITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QGraphicsScene>
#include <QDebug>

class QLayoutRectItem : public QGraphicsRectItem
{
public:
	explicit QLayoutRectItem(QGraphicsItem *parent = Q_NULLPTR);
	explicit QLayoutRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);

};

#endif // QLAYOUTRECTITEM_H
