#include "qgraphicsmacroitem.h"

QGraphicsMacroItem::QGraphicsMacroItem(QGraphicsItem* parent) :
	QGraphicsRectItem(parent),
	m_dragged(false)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

QGraphicsMacroItem::QGraphicsMacroItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent),
	   m_dragged(false)
{

}

void QGraphicsMacroItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	m_dragged = true;
	QGraphicsRectItem::mouseMoveEvent(event);
}

void QGraphicsMacroItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void QGraphicsMacroItem::setMacroName(QString n)
{
	macroName = n;
}

void QGraphicsMacroItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *markAction = menu.addAction("Mark");
	QAction *selectedAction = menu.exec(event->screenPos());
}
