#include "vcdviewgraphicsitemsignal.h"

VcdViewGraphicsItemSignal::VcdViewGraphicsItemSignal(vcd::Var var, vcd::TimeValues values, QGraphicsItem *parent) :
	VcdViewGraphicsItem(var,parent),
	m_isFirstValue(true)
{
	QGraphicsSimpleTextItem *text;
	QGraphicsLineItem *line;
	QPen sigPen;
	sigPen.setWidth(2);

	vcd::LogicValue lastValue;

	int lastTime = 0;
	int biggestTime = 0;
	int time;

	foreach(vcd::TimeValue value, values) {
		time = value.time();

		if(time>biggestTime)
			biggestTime = time;

		if(value.var_id()==var.id()) {
			addTime(time);

			sigPen.setColor(QColor(Qt::green));
			line = new QGraphicsLineItem(time, 0, time, height(), this);
			line->setPen(sigPen);

			if(m_isFirstValue) {
				if(value.value()==vcd::LogicValue::ONE) {
					sigPen.setColor(QColor(Qt::green));
					line = new QGraphicsLineItem(0, height(), time, height(), this);
					line->setPen(sigPen);
				} else if (value.value()==vcd::LogicValue::ZERO) {
					sigPen.setColor(QColor(Qt::green));
					line = new QGraphicsLineItem(0, 0, time, 0, this);
					line->setPen(sigPen);
				}
				m_isFirstValue = false;
			}

			if(lastValue==vcd::LogicValue::ONE) {
				sigPen.setColor(QColor(Qt::green));
				line = new QGraphicsLineItem(lastTime, 0, time, 0, this);
				line->setPen(sigPen);
			} else if (lastValue==vcd::LogicValue::ZERO) {
				sigPen.setColor(QColor(Qt::green));
				line = new QGraphicsLineItem(lastTime, height(), time, height(), this);
				line->setPen(sigPen);
			} else if (lastValue==vcd::LogicValue::HIGHZ) {
				sigPen.setColor(QColor(Qt::red));
				line = new QGraphicsLineItem(lastTime, height(), time, height(), this);
				line->setPen(sigPen);
			}

			lastValue = value.value();
			lastTime = time;
		}
	}
	if(lastValue==vcd::LogicValue::ONE) {
		sigPen.setColor(QColor(Qt::green));
		line = new QGraphicsLineItem(lastTime, 0, biggestTime, 0, this);
		line->setPen(sigPen);
	} else if (lastValue==vcd::LogicValue::ZERO) {
		sigPen.setColor(QColor(Qt::green));
		line = new QGraphicsLineItem(lastTime, height(), biggestTime, height(), this);
		line->setPen(sigPen);
	} else if (lastValue==vcd::LogicValue::HIGHZ) {
		sigPen.setColor(QColor(Qt::red));
		line = new QGraphicsLineItem(lastTime, height(), biggestTime, height(), this);
		line->setPen(sigPen);
	}
}
