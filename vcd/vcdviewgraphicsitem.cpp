#include "vcdviewgraphicsitem.h"

VcdViewGraphicsItem::VcdViewGraphicsItem(vcd::Var var, QGraphicsItem *parent) :
	QObject(),
	QGraphicsItem(parent),
	m_name(QString::fromStdString(var.name())),
	m_height(40),
	m_width(10),
	m_lastTime(10),
	m_firstTime(0),
	m_firstTimeSet(false)
{
	sigLabel = new QGraphicsSimpleTextItem(m_name, this);
	//sigLabel->setPos(recentZeroTime+space, drawingIndex*height+space);
	sigLabel->setBrush(QColor(Qt::white));
	sigLabel->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

	QPen pen;
	pen.setColor(Qt::gray);
	sigFrame = new QGraphicsRectItem(0,0,m_width,m_height,this);
	sigFrame->setPen(pen);

	m_long_name = "";
	foreach(std::string s, var.hierarchical_name()) {
		m_long_name+='/';
		m_long_name+=QString::fromStdString(s);
	}
}

void VcdViewGraphicsItem::addTime(qreal time)
{
	if(m_firstTimeSet) {
		if(time > m_lastTime) m_lastTime = time;
		if(time < m_firstTime) m_firstTime = time;
	} else {
		m_firstTime = time;
		m_lastTime = time;
		m_firstTimeSet = true;
	}
	m_width = m_lastTime;

	sigFrame->setRect(0,0,m_width,m_height);
}

QRectF VcdViewGraphicsItem::boundingRect() const
{
	return QRect(0,0,m_lastTime,m_height);
}

void VcdViewGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

qreal VcdViewGraphicsItem::height()
{
	return m_height;
}

qreal VcdViewGraphicsItem::width()
{
	return m_width;
}

bool VcdViewGraphicsItem::contains(const QPointF &point) const
{
	QPointF p = pos();
	QRectF r = boundingRect();
	r.setY(p.y());

	return r.contains(point);
}

QString VcdViewGraphicsItem::getLongName()
{
	return m_long_name;
}

void VcdViewGraphicsItem::deleteMe()
{
	emit(deleteInstance(this));
}
