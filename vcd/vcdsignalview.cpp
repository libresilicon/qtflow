#include "vcdsignalview.h"

#include <QDebug>

VcdSignalView::VcdSignalView(QWidget *parent) :
	QGraphicsView(parent),
	signalScene(new QGraphicsScene(this)),
	timeScale(1),
	recentZeroTime(0),
	moveDragLastX(0)
{
	setAcceptDrops(true);
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

void VcdSignalView::onZoomFitWidth()
{
	timeScale=1;
	redraw();
}

void VcdSignalView::onMoveRight()
{
	recentZeroTime++;
	redraw();
}

void VcdSignalView::onMoveLeft()
{
	if(recentZeroTime>1) recentZeroTime--;
	redraw();
}

void VcdSignalView::resizeEvent(QResizeEvent *event)
{
	redraw();
}

void VcdSignalView::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::MiddleButton)
	{
		QPointF pt = mapToScene(e->pos());
		moveDragLastX = pt.x();
	}
}

void VcdSignalView::mouseMoveEvent(QMouseEvent *e)
{
	int x;
	if (e->buttons() & Qt::MidButton)
	{
		QPointF pt = mapToScene(e->pos());
		x = pt.x();

		int newZeroTime=recentZeroTime+(x-moveDragLastX);
		int rightLimit=(timeScale*this->width()*highest_time/(highest_time-lowest_time))-signalScene->sceneRect().width();

		if((newZeroTime>=0)&&(newZeroTime<=rightLimit)) {
			recentZeroTime=newZeroTime;
			moveDragLastX=x;
		}
		redraw();
	}
}

void VcdSignalView::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	int newZeroTime=recentZeroTime+numSteps*100;
	int rightLimit=(timeScale*this->width()*highest_time/(highest_time-lowest_time))-signalScene->sceneRect().width();

	if((newZeroTime>=0)&&(newZeroTime<=rightLimit)) {
		recentZeroTime=newZeroTime;
		event->accept();
	}
	redraw();
}

void VcdSignalView::mouseReleaseEvent(QMouseEvent * e)
{
	//QPointF pt = mapToScene(e->pos());
	//int x = pt.x();
	//redraw();
	//signalScene->addLine(x,0,x,this->height(),QPen(Qt::red));

}

void VcdSignalView::mouseDoubleClickEvent(QMouseEvent * e)
{
	//QPointF pt = mapToScene(e->pos());
	//int x = pt.x();
	//redraw();
	//signalScene->addLine(x,0,x,this->height(),QPen(Qt::red));

}

void VcdSignalView::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	menu.addAction("Test");
	menu.exec(event->globalPos());
}

void VcdSignalView::dragEnterEvent(QDragEnterEvent *event)
{
}

void VcdSignalView::dragLeaveEvent(QDragLeaveEvent *e)
{
}

void VcdSignalView::dragMoveEvent(QDragMoveEvent *e)
{
	QPointF pt = mapToScene(e->pos());
	int x = pt.x();
	if(moveDragLastX<x) {
		onMoveRight();
		moveDragLastX = x;
	}
	if(moveDragLastX>x) {
		onMoveLeft();
		moveDragLastX = x;
	}
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
	rescale();
}

void VcdSignalView::rescale()
{
	signalScene->setSceneRect(recentZeroTime,0,this->width()-20,this->height()-20);
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
	} else if(scale.contains("ns")) {
		foreach(QChar c, scale) {
			if(c=='n') break;
			scaleValueString.append(c);
			scaleValue = scaleValueString.toInt();
		}
		for(int i=0; i < this->width()/10 ; i++) {
			signalScene->addLine(i*10, 0, i*10, 10, white);
		}
	} else if(scale.contains("s")) {
		foreach(QChar c, scale) {
			if(c=='s') break;
			scaleValueString.append(c);
			scaleValue = scaleValueString.toInt();
		}
		for(int i=0; i < this->width()/10 ; i++) {
			signalScene->addLine(i*10, 0, i*10, 10, white);
		}
	} else {
		qDebug() << "(not supported) time scale: " << scale;
	}
}

void VcdSignalView::drawSignalBus(QString signal_name, int idx)
{
	if(!mapIdName.contains(signal_name)) return;

	bool drawn = false;

	int height = this->height()-20;
	height /= signalViewFilter.count();
	int space = height/10;

	QPen sigPen(Qt::green);

	vcd::LogicValue lastValue;
	int lastTime = 0;
	int time;

	qDebug() << "bus.var_id()" << vcd_data.time_bus_values().size();

	foreach(vcd::TimeBusValue bus, vcd_data.time_bus_values()) {
		if(bus.var_id()==mapIdName[signal_name]) {
			time = timeScale*this->width()*(bus.time()-lowest_time)/(highest_time-lowest_time);
			sigPen.setColor(Qt::green);
			if((time-RAISE_TIME)>0) {
				signalScene->addLine(time+RAISE_TIME, idx*height+space, time-RAISE_TIME, (idx+1)*height-space, sigPen);
				signalScene->addLine(time-RAISE_TIME, idx*height+space, time+RAISE_TIME, (idx+1)*height-space, sigPen);
				signalScene->addLine(lastTime+RAISE_TIME, idx*height+space, time-RAISE_TIME, idx*height+space, sigPen);
				signalScene->addLine(lastTime+RAISE_TIME, (idx+1)*height-space, time-RAISE_TIME, (idx+1)*height-space, sigPen);
			} else {
				signalScene->addLine(0, (idx*height)+(height/2), RAISE_TIME, (idx*height)+space, sigPen);
				signalScene->addLine(0, (idx*height)+(height/2), RAISE_TIME, (idx*height)+height-space, sigPen);
			}

			/*foreach(vcd::LogicValue value, bus.values()) {
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
			}*/

			drawn = true;
			lastTime = time;
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

	int height = this->height()-20;
	height /= signalViewFilter.count();
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
