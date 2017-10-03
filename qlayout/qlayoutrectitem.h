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

	bool contains(const QPointF &point) const Q_DECL_OVERRIDE;
	bool isLocked();

	qreal rectWidth() const;
	qreal rectHeight() const;
	qreal rectX() const;
	qreal rectY() const;

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	QPointF m_orig;
	bool m_locked;
};

#endif // QLAYOUTRECTITEM_H
