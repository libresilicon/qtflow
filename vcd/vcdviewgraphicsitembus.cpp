#include "vcdviewgraphicsitembus.h"

VcdViewGraphicsItemBus::VcdViewGraphicsItemBus(vcd::Var var, vcd::TimeBusValues values, QGraphicsItem *parent) :
	VcdViewGraphicsItem(var,parent)
{
	QPen sigPen(Qt::green);

	int i;
	int lastTime = 0;
	int time;
	QString lastBusValueStd;
	QVector<vcd::LogicValue> reverse;
	QGraphicsLineItem *line;
	QGraphicsSimpleTextItem *text;

	char busValueStd[var.width()+1];

	foreach(vcd::TimeBusValue bus, values) {
		time = bus.time();
		addTime(time);
		if(bus.var_id()==var.id()) {
			busValueStd[var.width()]='\0';
			for(i=0;i<var.width();i++) {
				busValueStd[i]='0';
			}

			reverse.clear();
			foreach(vcd::LogicValue value, bus.values())
				reverse.prepend(value);

			i = var.width()-1;
			foreach(vcd::LogicValue value, reverse) {
				if(value==vcd::LogicValue::ONE) {
					busValueStd[i]='1';
				} else if(value==vcd::LogicValue::ZERO) {
					busValueStd[i]='0';
				}
				i--;
			}

			sigPen.setColor(Qt::green);

			line = new QGraphicsLineItem(time+RAISE_TIME, 0, time-RAISE_TIME, height(), this);
			line->setPen(sigPen);

			line = new QGraphicsLineItem(time-RAISE_TIME, 0, time+RAISE_TIME, height(), this);
			line->setPen(sigPen);

			line = new QGraphicsLineItem(lastTime+RAISE_TIME, 0, time-RAISE_TIME, 0, this);
			line->setPen(sigPen);

			line = new QGraphicsLineItem(lastTime+RAISE_TIME, height(), time-RAISE_TIME, height(), this);
			line->setPen(sigPen);

			text = new QGraphicsSimpleTextItem(QString("%1").arg(QString(busValueStd).toInt(0,2)),this);
			text->setPos(lastTime+(time-lastTime)/2,height()/3);
			text->setBrush(QColor(Qt::white));
			text->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

			lastBusValueStd = busValueStd;
			/*else {
				signalScene->addLine(lastTime, drawingIndex*height+space*2, time+raise_time, drawingIndex*height+space*2, sigPen);
				signalScene->addLine(lastTime, (drawingIndex+1)*height-space*2, time+raise_time, (drawingIndex+1)*height-space*2, sigPen);
			}*/

			//drawn = true;
			lastTime = time;
		}
	}

	/*if(drawn) {
		// box around bus signal:
		signalScene->addRect(0, drawingIndex*height+space, lastTime, height-space*2, QPen(Qt::white));

		// add it to the list
		busSignalAreas[signal_name].area = QRect(0, drawingIndex*height+space, lastTime, height-space*2);

		// name of the signal bus:
		text = signalScene->addSimpleText(signal_name);
		text->setPos(recentZeroTime+space, drawingIndex*height+space);
		text->setBrush(QColor(Qt::white));
		text->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

		drawingIndex++;

		if(busSignalAreas[signal_name].isUnfolded) {
			drawSubSignals(signal_name);
		}

		return true;
	}*/
}


