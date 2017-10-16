#include "vcdviewgraphicsitemsignal.h"

VcdViewGraphicsItemSignal::VcdViewGraphicsItemSignal(vcd::Var var, vcd::TimeValues values, QGraphicsItem *parent) :
	VcdViewGraphicsItem(var,parent)
{
	QGraphicsSimpleTextItem *text;
	QGraphicsLineItem *line;
	QPen sigPen;

	vcd::LogicValue lastValue;

	bool drawn = false;

	int lastTime = 0;
	int time;
	foreach(vcd::TimeValue value, values) {
		if(value.var_id()==var.id()) {
			time = value.time();
			addTime(time);

			sigPen.setColor(QColor(Qt::green));
			line = new QGraphicsLineItem(time, 0, time, height(), this);
			line->setPen(sigPen);

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
			drawn = true;
		}
	}

	if(drawn) {
		// box for the signal
		//signalScene->addRect(0, drawingIndex*height+space, lastTime, height-space*2, QPen(Qt::white));

		// add it to the list
		//signalAreas[signal_name]=QRect(0, drawingIndex*height+space, lastTime, height-space*2);

		// name of the signal bus:
		//text = signalScene->addSimpleText(signal_name);
		//text->setPos(recentZeroTime+space, drawingIndex*height+space);
		//text->setBrush(QColor(Qt::white));
		//text->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

		//drawingIndex++;
	}
}
