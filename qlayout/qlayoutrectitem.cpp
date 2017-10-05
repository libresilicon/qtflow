#include "qlayoutrectitem.h"

QLayoutRectItem::QLayoutRectItem(QGraphicsItem* parent) :
	QGraphicsItem(parent),
	m_locked(false),
	m_color(Qt::black),
	m_recentCutOutRectangle(NULL)
{
}

QLayoutRectItem::QLayoutRectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent) :
	  QGraphicsItem(parent),
	   m_locked(false),
	   m_color(Qt::black),
	   m_externalRect(QRectF(x,y,w,h)),
	   m_recentCutOutRectangle(NULL)
{
}

bool QLayoutRectItem::contains(const QPointF &point) const
{
	QPointF offsetOrig;
	offsetOrig.setX(point.x()-x());
	offsetOrig.setY(point.y()-y());

	return m_externalRect.contains(offsetOrig);
}

void QLayoutRectItem::setColor(QColor c)
{
	m_color = c;
}

qreal QLayoutRectItem::width() const
{
	return m_externalRect.width();
}

qreal QLayoutRectItem::height() const
{
	return m_externalRect.height();
}

bool QLayoutRectItem::isLocked()
{
	return m_locked;
}

void QLayoutRectItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *markAction = menu.addAction("Mark");
	QAction *selectedAction = menu.exec(event->screenPos());
}

void QLayoutRectItem::setCutOutStart(qreal x, qreal y)
{
	foreach(QRectF *m, m_cutoutRectangles) {
		if(m->contains(QPointF(x,y))) return;
	}
	m_recentCutOutRectangle = new QRectF(x,y,1,1);
	m_cutoutRectangles.append(m_recentCutOutRectangle);
}

void QLayoutRectItem::updateRecentCutOut(qreal w, qreal h)
{
	QRectF *m;
	qreal xmax, ymax;
	qreal xmin, ymin;
	qreal x, y;
	QPointF pt;

	if(m_recentCutOutRectangle) {
		xmin = m_externalRect.x();
		xmax = m_externalRect.x() + m_externalRect.width();
		ymin = m_externalRect.y();
		ymax = m_externalRect.y() + m_externalRect.height();

		x = m_recentCutOutRectangle->x() + w;
		y = m_recentCutOutRectangle->y() + h;
		pt = QPointF(x,y);

		foreach(m, m_cutoutRectangles)
			if(m->contains(pt))
				return;

		if((x>=xmin)&&(x<=xmax)&&(y>=ymin)&&(y<=ymax)) {
			m_recentCutOutRectangle->setWidth(w);
			m_recentCutOutRectangle->setHeight(h);
		}
	}
}

void QLayoutRectItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
	m_externalRect = QRectF(x,y,w,h);
}

QRectF QLayoutRectItem::boundingRect() const
{
	return m_externalRect;
}

QVector<QRectF> QLayoutRectItem::cutOutRectangleFromGroup(QVector<QRectF> group, QRectF cutout)
{
	QVector<QRectF> ret;
	QPointF ctr = cutout.topRight();
	QPointF ctl = cutout.topLeft();
	QPointF cbr = cutout.bottomRight();
	QPointF cbl = cutout.bottomLeft();

	QPointF tr;
	QPointF tl;
	QPointF br;
	QPointF bl;

	foreach(QRectF rect, group) {
		tr = rect.topRight();
		tl = rect.topLeft();
		br = rect.bottomRight();
		bl = rect.bottomLeft();

		// points from top left to bottom right:

		// fill corners
		ret.append(QRectF(tl,ctl)); // left upper corner
		ret.append(QRectF(cbr,br)); // right lower corner
		ret.append(QRectF(QPointF(ctr.x(),tr.y()),QPointF(tr.x(),ctr.y()))); // right upper corner
		ret.append(QRectF(QPointF(bl.x(),cbl.y()),QPointF(cbl.x(),bl.y()))); // left lower corner

		// fill stripes:
		ret.append(QRectF(QPointF(ctl.x(),tl.y()),ctr)); // upper stripe
		ret.append(QRectF(cbl,QPointF(cbr.x(),br.y()))); // lower stripe
		ret.append(QRectF(QPointF(tl.x(),ctl.y()),cbl)); // left stripe
		ret.append(QRectF(ctr,QPointF(br.x(),cbr.y()))); // right stripe

	}

	return ret;
}

QVector<QRectF> QLayoutRectItem::getStripes()
{
	qreal x,y,w,h;

	QVector<QRectF> ret;

	if(m_cutoutRectangles.count()) {
		ret.append(m_externalRect);
		foreach(QRectF *m, m_cutoutRectangles) {
			ret = cutOutRectangleFromGroup(ret, *m);
		}
	} else {
		x = m_externalRect.x() + pos().x();
		y = m_externalRect.y() + pos().y();
		w = m_externalRect.width();
		h = m_externalRect.height();
		ret.append(QRectF(x,y,w,h));
	}

	return ret;
}

void QLayoutRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(QPen(m_color));
	painter->setBrush(QBrush(m_color));
	painter->drawPath(shape());
}

QPainterPath QLayoutRectItem::shape() const
{
	QPainterPath path;
	path.addRect(m_externalRect);

	foreach(QRectF *r, m_cutoutRectangles)
		path.addRect(*r);

	return path; // return the item's defined shape
}
