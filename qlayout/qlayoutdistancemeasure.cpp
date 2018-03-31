#include "qlayoutdistancemeasure.h"

QLayoutDistanceMeasure::QLayoutDistanceMeasure(QString s, QPointF p1, QPointF p2, QGraphicsItem *parent) :
	QGraphicsItem(parent),
	m_label(NULL),
	m_mainLine(NULL)
{
	m_mainLine = new QGraphicsLineItem(QLineF(p1,p2), this);
	m_label = new QGraphicsSimpleTextItem(s, m_mainLine);
}

QLayoutDistanceMeasure::QLayoutDistanceMeasure(QGraphicsItem *parent) :
	QGraphicsItem(parent),
	m_label(NULL),
	m_mainLine(NULL)
{
}

void QLayoutDistanceMeasure::updateLabel(QString s)
{
	if(m_mainLine) {
		if(m_label==NULL) {
			m_label = new QGraphicsSimpleTextItem(this);
		}
		m_label->setText(s);
		m_label->setPos((m_p2+m_p1)/2);
		qDebug() << m_label->pos();	
	}
}

void QLayoutDistanceMeasure::setLine(qreal x1, qreal y1, qreal x2, qreal y2)
{
	if(m_mainLine==NULL) {
		m_mainLine = new QGraphicsLineItem(this);
	}
	m_mainLine->setLine(x1,y1,x2,y2);
	m_p1 = QPointF(x1,y1);
	m_p2 = QPointF(x2,y2);
}

QRectF QLayoutDistanceMeasure::boundingRect() const
{
}

void QLayoutDistanceMeasure::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
