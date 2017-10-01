#ifndef QGRAPHICSMACROITEM_H
#define QGRAPHICSMACROITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>

class QGraphicsMacroItem : public QGraphicsRectItem
{
public:
	explicit QGraphicsMacroItem(QGraphicsItem *parent = Q_NULLPTR);
	explicit QGraphicsMacroItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);

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

#endif // QGRAPHICSMACROITEM_H
