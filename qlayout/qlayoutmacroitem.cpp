#include "qlayoutmacroitem.h"

QLayoutMacroItem::QLayoutMacroItem(QGraphicsItem* parent) :
	QGraphicsRectItem(parent),
	m_dragged(false)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

QLayoutMacroItem::QLayoutMacroItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent),
	   m_dragged(false)
{

}

void QLayoutMacroItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	m_dragged = true;
	QGraphicsRectItem::mouseMoveEvent(event);
}

void QLayoutMacroItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void QLayoutMacroItem::setMacroName(QString n)
{
	macroName = n;
}

QString QLayoutMacroItem::getMacroName()
{
	return macroName;
}

void QLayoutMacroItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *markAction = menu.addAction("Mark");
	QAction *selectedAction = menu.exec(event->screenPos());
}
