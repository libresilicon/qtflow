#include "qlayoutmacroitem.h"

QLayoutMacroItem::QLayoutMacroItem(qreal scale, QLayoutMacroItem *orig) : // cloning the template object and it's children
	QGraphicsRectItem(0),
	m_dragged(false),
	m_instanceNameLabel(NULL),
	m_width(1),
	m_height(1)
{
	QRectF rect;
	QPen p = pen();
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	p.setWidth(1);
	p.setColor(Qt::black);
	p.setCosmetic(true);
	setPen(p);

	rect = orig->rect();
	rect = QRectF(rect.x()*scale,rect.y()*scale,rect.width()*scale,rect.height()*scale);
	setRect(rect);
	m_width = rect.width();
	m_height = rect.height();

	setInstanceName(orig->m_instanceName);
	setMacroName(orig->m_macroName);

	foreach(QString layer, orig->m_layerList.keys()) {
		foreach(QGraphicsRectItem* m, orig->m_layerList[layer]) {
			if(m) {
				rect = m->rect();
				rect = QRectF(rect.x()*scale,rect.y()*scale,rect.width()*scale,rect.height()*scale);
				addRectangle(layer,m->brush(),rect);
			}
		}
	}

	foreach(QLayoutMacroPinItem* pi, orig->m_pinList) {
		m_pinList.append(new QLayoutMacroPinItem(scale, pi,this));
	}
}

QLayoutMacroItem::QLayoutMacroItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	   QGraphicsRectItem(x,y,w,h,parent),
	   m_dragged(false)
{
}

void QLayoutMacroItem::addRectangle(QString layer, QBrush brush, QRectF rect)
{
	if(layer==QString())
		return;

	QPen p;
	QGraphicsRectItem* r = new QGraphicsRectItem(rect,this);
	r->setVisible(true);
	r->setBrush(brush);
	r->setOpacity(0.75);
	p = r->pen();
	p.setCosmetic(true);
	r->setPen(p);
	m_layerList[layer].append(r);
}

void QLayoutMacroItem::addPolygon(QString layer, QBrush brush, QPolygonF poly)
{
	if(layer==QString())
		return;

	QPen p;
	QGraphicsRectItem* r = new QGraphicsRectItem(poly.boundingRect(),this);
	r->setVisible(true);
	r->setBrush(brush);
	r->setOpacity(0.75);
	p = r->pen();
	p.setCosmetic(true);
	r->setPen(p);
	m_layerGDSList[layer].append(r);
}

void QLayoutMacroItem::clearPolygons()
{
	int i;
	foreach(QString layer, m_layerGDSList.keys()) {
		foreach(QGraphicsRectItem* r, m_layerGDSList[layer]) {
			i = m_layerGDSList[layer].indexOf(r);
			m_layerGDSList[layer].remove(i);
			delete r;
		}
	}
}

void QLayoutMacroItem::clearRectangles()
{
	int i;
	foreach(QString layer, m_layerList.keys()) {
		foreach(QGraphicsRectItem* r, m_layerList[layer]) {
			i = m_layerList[layer].indexOf(r);
			m_layerList[layer].remove(i);
			delete r;
		}
	}
}

void QLayoutMacroItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	m_dragged = true;
	QGraphicsRectItem::mouseMoveEvent(event);
}

void QLayoutMacroItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

QString QLayoutMacroItem::getMacroName()
{
	return m_macroName;
}

QLayoutMacroPinItem* QLayoutMacroItem::addPin(QString name)
{
	QLayoutMacroPinItem* pi;
	pi = new QLayoutMacroPinItem(this);
	pi->setName(name);
	m_pinList.append(pi);
	return pi;
}

void QLayoutMacroItem::setInstanceName(QString n)
{
	m_instanceName = n;

	m_instanceNameLabel = new QGraphicsSimpleTextItem(m_instanceName,this);
	m_instanceNameLabel->setVisible(true);

	m_instanceNameLabel->setScale(rect().width()/m_instanceNameLabel->boundingRect().width());
}

void QLayoutMacroItem::setMacroName(QString n)
{
	m_macroName = n;

	m_macroNameLabel = new QGraphicsSimpleTextItem(m_macroName,this);
	m_macroNameLabel->setVisible(true);

	m_macroNameLabel->setScale(rect().width()/m_macroNameLabel->boundingRect().width());
}

void QLayoutMacroItem::setSize(qreal w, qreal h)
{
	qreal scaleFactor;

	scaleFactor = (m_width>m_height)?(qFabs(w)/m_width):(qFabs(h)/m_height);
	m_width *= scaleFactor;
	m_height *= scaleFactor;

	setScale(scaleFactor);

	//if(w<0) setPos(pos().x()+w,pos().y());
	//if(h<0) setPos(pos().x(),pos().y()+h);
}

QString QLayoutMacroItem::getInstanceName()
{
	return m_instanceName;
}

void QLayoutMacroItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *markAction = menu.addAction("Mark");
	QAction *selectedAction = menu.exec(event->screenPos());
}
