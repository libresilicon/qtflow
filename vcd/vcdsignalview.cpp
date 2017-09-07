#include "vcdsignalview.h"

#include <QDebug>

VcdSignalView::VcdSignalView(QWidget *parent) :
	QGraphicsView(parent),
	signalScene(new QGraphicsScene(this)),
	timeScale(1)
{
	signalScene->setBackgroundBrush(Qt::black);
	setScene(signalScene);
}


void VcdSignalView::onZoomIn()
{
	timeScale++;
	redraw();
}

void VcdSignalView::onZoomOut()
{
	if(timeScale>1) timeScale--;
	redraw();
}

void VcdSignalView::mousePressEvent(QMouseEvent * e)
{
	QPointF pt = mapToScene(e->pos());
	int x = pt.x();
	redraw();
	signalScene->addLine(x,0,x,this->height(),QPen(Qt::red));

}

void VcdSignalView::mouseDoubleClickEvent(QMouseEvent * e)
{
	QPointF pt = mapToScene(e->pos());
	int x = pt.x();
	//redraw();
	//signalScene->addLine(x,0,x,this->height(),QPen(Qt::red));

}

void VcdSignalView::contextMenuEvent(QContextMenuEvent *event)
{

}

void VcdSignalView::dragEnterEvent(QDragEnterEvent *event)
{

}

void VcdSignalView::dragLeaveEvent(QDragLeaveEvent *event)
{

}

void VcdSignalView::dragMoveEvent(QDragMoveEvent *event)
{

}

void VcdSignalView::dropEvent(QDropEvent *event)
{

}

void VcdSignalView::setVCD(vcd::VcdData d)
{
	QString hname;
	vcd_data = d;
	mapIdName.clear();
	foreach(vcd::Var var, vcd_data.vars()) {
		hname = "";
		foreach(std::string sname, var.hierarchical_name()) {
			hname+='/';
			hname+=QString::fromStdString(sname);
		}
		mapIdName[hname]=var.id();
	}

	highest_time = 0;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
		if(value.time()>highest_time) highest_time = value.time();
	}

	lowest_time = highest_time;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
		if(value.time()<lowest_time) lowest_time = value.time();
	}
}

void VcdSignalView::append(QString s)
{
	if(!signalViewFilter.contains(s)) {
		signalViewFilter.append(s);
	}
	redraw();
}

void VcdSignalView::redraw()
{
	signalScene->clear();
	drawTimeScale();
	int idx = 0;
	foreach(QString s, signalViewFilter) {
		drawSignal(s,idx);
		drawSignalBus(s,idx);
		idx++;
	}
}

void VcdSignalView::drawTimeScale()
{
	QPen white(Qt::white);
	QString orig = QString::fromStdString(vcd_data.header().timescale());
	QString scale = "";
	QString scaleValueString = "";
	int scaleValue = 1;
	foreach(QChar c, orig) {
		if(c!='\x9') {
			scale.append(c);
		}
	}
	if(scale.contains("ps")) {
		foreach(QChar c, scale) {
			if(c=='p') break;
			scaleValueString.append(c);
			scaleValue = scaleValueString.toInt();
		}
		for(int i=0; i < this->width()/10 ; i++) {
			signalScene->addLine(i*10, 0, i*10, 10, white);
		}
	}
}

void VcdSignalView::drawSignalBus(QString signal_name, int idx)
{
	if(!mapIdName.contains(signal_name)) return;

	bool drawn = false;

	int height = this->height();
	height /= signalViewFilter.count();
	height /= 2;
	int space = height/10;

	QPen sigPen(Qt::green);

	vcd::LogicValue lastValue;
	int lastTime = 0;
	int time;

	qDebug() << "bus.var_id()" << vcd_data.time_bus_values().size();

	foreach(vcd::TimeBusValue bus, vcd_data.time_bus_values()) {
		if(bus.var_id()==mapIdName[signal_name]) {
			foreach(vcd::LogicValue value, bus.values()) {
				time = timeScale*this->width()*(bus.time()-lowest_time)/(highest_time-lowest_time);
				sigPen.setColor(Qt::green);
				signalScene->addLine(time, idx*height+space, time, (idx+1)*height-space, sigPen);
				if(lastValue==vcd::LogicValue::ONE) {
					sigPen.setColor(Qt::green);
					signalScene->addLine(lastTime, (idx+1)*height-space, time, (idx+1)*height-space , sigPen);
				} else if (lastValue==vcd::LogicValue::ZERO) {
					sigPen.setColor(Qt::green);
					signalScene->addLine(lastTime, idx*height+space, time, idx*height+space, sigPen);
				} else if (lastValue==vcd::LogicValue::HIGHZ) {
					sigPen.setColor(Qt::red);
					signalScene->addLine(lastTime, idx*height+space, time, idx*height+space, sigPen);
				}
				lastValue = value;
				lastTime = time;
			}
			drawn = true;
		}
	}

	if(drawn) {
		signalScene->addRect(0, idx*height, lastTime, height, QPen(Qt::white));
	}
}

void VcdSignalView::drawSignal(QString signal_name, int idx)
{
	if(!mapIdName.contains(signal_name)) return;

	bool drawn = false;

	int height = this->height();
	height /= signalViewFilter.count();
	height /= 2;
	int space = height/10;

	QPen sigPen(Qt::green);

	vcd::LogicValue lastValue;
	int lastTime = 0;
	int time;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
		if(value.var_id()==mapIdName[signal_name]) {
			time = timeScale*this->width()*(value.time()-lowest_time)/(highest_time-lowest_time);
			sigPen.setColor(Qt::green);
			signalScene->addLine(time, idx*height+space, time, (idx+1)*height-space, sigPen);
			if(lastValue==vcd::LogicValue::ONE) {
				sigPen.setColor(Qt::green);
				signalScene->addLine(lastTime, (idx+1)*height-space, time, (idx+1)*height-space , sigPen);
			} else if (lastValue==vcd::LogicValue::ZERO) {
				sigPen.setColor(Qt::green);
				signalScene->addLine(lastTime, idx*height+space, time, idx*height+space, sigPen);
			} else if (lastValue==vcd::LogicValue::HIGHZ) {
				sigPen.setColor(Qt::red);
				signalScene->addLine(lastTime, idx*height+space, time, idx*height+space, sigPen);
			}
			lastValue = value.value();
			lastTime = time;
			drawn = true;
		}
	}

	if(drawn) {
		signalScene->addRect(0, idx*height, lastTime, height, QPen(Qt::white));
	}
}
