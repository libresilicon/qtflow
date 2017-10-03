#ifndef QLAYOUTMACROITEM_H
#define QLAYOUTMACROITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>

class QLayoutMacroItem : public QGraphicsRectItem
{
public:
	explicit QLayoutMacroItem(QGraphicsItem *parent = Q_NULLPTR);
	explicit QLayoutMacroItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);

	void setMacroName(QString);
	QString getMacroName();

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:

public slots:

private:
	bool m_dragged;
	QString macroName;
};

#endif // QLAYOUTMACROITEM_H
