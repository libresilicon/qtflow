#include "qschematicspin.h"

QSchematicsPin::QSchematicsPin(QString name, int index, int x, int y, int length, QString orient, int tw, int th, int a, int b, QString mode, QGraphicsItem *p) :
	QGraphicsItem(p)
{
	m_pinLine = new QGraphicsLineItem(drawLine(x,-y,length,orient),p);

	// adding label:
	m_pinLabel = new QGraphicsSimpleTextItem(name,p);
	if(orient=="U") {
		m_pinLabel->setPos(x+5,-y-length/2);
	} else if(orient=="D") {
		m_pinLabel->setPos(x+5,-y+length/2);
	} else if(orient=="L") {
		m_pinLabel->setPos(x-length/2,-y);
	} else if(orient=="R") {
		m_pinLabel->setPos(x+length/2,-y);
	}

	// adding contact:
	m_pinContact = new QGraphicsRectItem(x-4,-4-y,8,8,p);
	m_pinContact->setBrush(QBrush(Qt::black));

}

void QSchematicsPin::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

QRectF QSchematicsPin::boundingRect() const
{
	return m_externalRect;
}

QLineF QSchematicsPin::drawLine(int x, int y, int length, QString orient)
{
	QPointF p1(x,y);
	QPointF p2(x,y+length);

	if(orient=="U") {
		p2 = QPointF(x,y-length);
	} else if(orient=="D") {
		p2 = QPointF(x,y+length);
	} else if(orient=="L") {
		p2 = QPointF(x-length,y);
	} else if(orient=="R") {
		p2 = QPointF(x+length,y);
	}

	return QLineF(p1,p2);
}
