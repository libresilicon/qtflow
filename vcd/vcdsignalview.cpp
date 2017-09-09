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

void VcdSignalView::contextMenuEvent(QContextMenuEvent *e)
{
	int y1, y2;
	QPointF pt = mapToScene(e->pos());
	QMenu menu(this);
	QAction *action;

	foreach(QString key, busSignalAreas.keys()) {
		y1 = busSignalAreas[key].area.y();
		y2 = y1+busSignalAreas[key].area.height();
		if((pt.y()>=y1)&&(pt.y()<=y2)) {
			action = menu.addAction("Remove signal bus");
			action->setData(key);
			connect(action,SIGNAL(triggered(bool)),this, SLOT(onRemoveSignal()));

			if(busSignalAreas[key].isUnfolded) {
				action = menu.addAction("Fold bus");
				action->setData(key);
				connect(action,SIGNAL(triggered(bool)),this, SLOT(onFoldSignalBus()));
			} else {
				action = menu.addAction("Unfold bus");
				action->setData(key);
				connect(action,SIGNAL(triggered(bool)),this, SLOT(onUnFoldSignalBus()));
			}
		}
	}

	foreach(QString key, signalAreas.keys()) {
		y1 = signalAreas[key].y();
		y2 = y1+signalAreas[key].height();
		if((pt.y()>=y1)&&(pt.y()<=y2)) {
			action = menu.addAction("Remove signal");
			action->setData(key);
			connect(action,SIGNAL(triggered(bool)),this, SLOT(onRemoveSignal()));
		}
	}

	menu.exec(e->globalPos());
}

void VcdSignalView::onFoldSignalBus()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QString key = action->data().toString();

	if(busSignalAreas.contains(key)) {
		busSignalAreas[key].isUnfolded = false;
	}

	redraw();
}

void VcdSignalView::onUnFoldSignalBus()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QString key = action->data().toString();

	if(busSignalAreas.contains(key)) {
		busSignalAreas[key].isUnfolded = true;
	}

	redraw();
}

void VcdSignalView::onRemoveSignal()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QString key = action->data().toString();

	for(int i=0;i<signalViewFilter.count();i++) {
		if(signalViewFilter.at(i)==key) {
			signalViewFilter.removeAt(i);
		}
	}

	redraw();
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

	drawingIndex = 0;
	foreach(QString s, signalViewFilter) {
		drawSignalBus(s);
		drawSignal(s);
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

QString VcdSignalView::getHierarchyNameString(std::vector<std::string> l)
{
	QString ret = "";
	std::string s;
	foreach(s,l) {
		ret+='/';
		ret+=QString::fromStdString(s);
	}
	return ret;
}

bool VcdSignalView::drawSignalBus(QString signal_name)
{
	if(!mapIdName.contains(signal_name)) return false;

	bool unfold = false;
	int height;
	QString busValue;
	QGraphicsSimpleTextItem *text;
	int width = 1;
	foreach(vcd::Var var, vcd_data.vars()) {
		if(getHierarchyNameString(var.hierarchical_name())==signal_name) {
			width = var.width();
		}
	}

	bool drawn = false;

	if(busSignalAreas.contains(signal_name)) {
		unfold=busSignalAreas[signal_name].isUnfolded;
	}

	int divisor = signalViewFilter.count();
	if(unfold) {
		divisor += width;
	}

	height = this->height()-10;
	height /= divisor;

	int space = height;
	space /= 10;

	QPen sigPen(Qt::green);

	vcd::LogicValue lastValue;
	int lastTime = 0;
	int time;

	foreach(vcd::TimeBusValue bus, vcd_data.time_bus_values()) {
		if(bus.var_id()==mapIdName[signal_name]) {
			time = timeScale*this->width()*(bus.time()-lowest_time)/(highest_time-lowest_time);
			sigPen.setColor(Qt::green);
			if((time-RAISE_TIME)>0) {
				signalScene->addLine(time+RAISE_TIME, drawingIndex*height+space+BOX_SPACE, time-RAISE_TIME, (drawingIndex+1)*height-space-BOX_SPACE, sigPen);
				signalScene->addLine(time-RAISE_TIME, drawingIndex*height+space+BOX_SPACE, time+RAISE_TIME, (drawingIndex+1)*height-space-BOX_SPACE, sigPen);
				signalScene->addLine(lastTime+RAISE_TIME, drawingIndex*height+space+BOX_SPACE, time-RAISE_TIME, drawingIndex*height+space+BOX_SPACE, sigPen);
				signalScene->addLine(lastTime+RAISE_TIME, (drawingIndex+1)*height-space-BOX_SPACE, time-RAISE_TIME, (drawingIndex+1)*height-space-BOX_SPACE, sigPen);
			} else {
				signalScene->addLine(0, (drawingIndex*height)+BOX_SPACE+space+((height-2*(BOX_SPACE+space))/2), RAISE_TIME, (drawingIndex*height)+space+BOX_SPACE, sigPen);
				signalScene->addLine(0, (drawingIndex*height)+BOX_SPACE+space+((height-2*(BOX_SPACE+space))/2), RAISE_TIME, (drawingIndex*height)+height-BOX_SPACE-space, sigPen);
			}

			int i;
			busValue = "0b";
			char busValueStd[width+1];
			busValueStd[width]='\0';
			for(i=0;i<width;i++) {
				busValueStd[i]='0';
			}

			i = 1;
			foreach(vcd::LogicValue value, bus.values()) {
				if(value==vcd::LogicValue::ONE) {
					busValueStd[width-i]='1';
				} else if (value==vcd::LogicValue::ZERO) {
					busValueStd[width-i]='0';
				} else if (value==vcd::LogicValue::HIGHZ) {
					busValueStd[width-i]='z';
				} else if (value==vcd::LogicValue::UNKOWN) {
					busValueStd[width-i]='x';
				}
				i++;
			}
			busValue += busValueStd;
			text = signalScene->addSimpleText(busValue);
			text->setPos(time+BUS_VALUE_SPACING, (drawingIndex*height)+(height/2));
			text->setBrush(QColor(Qt::white));

			drawn = true;
			lastTime = time;
		}
	}

	if(drawn) {
		// box around bus signal:
		signalScene->addRect(0, drawingIndex*height+BOX_SPACE, lastTime, height-BOX_SPACE*2, QPen(Qt::white));

		// add it to the list
		busSignalAreas[signal_name]=SignalBusArea();
		busSignalAreas[signal_name].area=QRect(0, drawingIndex*height+BOX_SPACE, lastTime, height-BOX_SPACE*2);
		busSignalAreas[signal_name].isUnfolded = unfold;

		// name of the signal bus:
		text = signalScene->addSimpleText(signal_name);
		text->setPos(recentZeroTime+space, drawingIndex*height+BOX_SPACE);
		text->setBrush(QColor(Qt::white));

		drawingIndex++;

		if(unfold) {
			drawSubSignals(signal_name);
		}

		return true;
	}

	return false;
}

bool VcdSignalView::drawSubSignals(QString signal_name)
{
	if(!mapIdName.contains(signal_name)) return false;

	int height;
	int width = 1;
	foreach(vcd::Var var, vcd_data.vars()) {
		if(getHierarchyNameString(var.hierarchical_name())==signal_name) {
			width = var.width();
		}
	}

	height = this->height()-10;
	height /= signalViewFilter.count();

	int space = height;
	space /= 10;

	QPen sigPen(Qt::green);

	vcd::LogicValue lastValue;
	int lastTime = 0;
	int time;

	int idx;
	foreach(vcd::TimeBusValue bus, vcd_data.time_bus_values()) {
		if(bus.var_id()==mapIdName[signal_name]) {
			qDebug() << "Drawing: " << signal_name;

			idx = drawingIndex;
			time = timeScale*this->width()*(bus.time()-lowest_time)/(highest_time-lowest_time);
			foreach(vcd::LogicValue value, bus.values()) {
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
				idx++;
			}

			lastTime = time;
		}
	}
}

bool VcdSignalView::drawSignal(QString signal_name)
{
	if(!mapIdName.contains(signal_name)) return false;

	bool drawn = false;
	int height;
	QGraphicsSimpleTextItem *text;

	height = this->height()-10;
	height /= signalViewFilter.count();

	int space = height;
	space /= 10;

	QPen sigPen(Qt::green);

	vcd::LogicValue lastValue;
	int lastTime = 0;
	int time;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
		if(value.var_id()==mapIdName[signal_name]) {
			time = timeScale*this->width()*(value.time()-lowest_time)/(highest_time-lowest_time);
			sigPen.setColor(Qt::green);
			signalScene->addLine(time, drawingIndex*height+space+BOX_SPACE, time, (drawingIndex+1)*height-space-BOX_SPACE, sigPen);
			if(lastValue==vcd::LogicValue::ONE) {
				sigPen.setColor(Qt::green);
				signalScene->addLine(lastTime, (drawingIndex+1)*height-space-BOX_SPACE, time, (drawingIndex+1)*height-space-BOX_SPACE , sigPen);
			} else if (lastValue==vcd::LogicValue::ZERO) {
				sigPen.setColor(Qt::green);
				signalScene->addLine(lastTime, drawingIndex*height+space+BOX_SPACE, time, drawingIndex*height+space+BOX_SPACE, sigPen);
			} else if (lastValue==vcd::LogicValue::HIGHZ) {
				sigPen.setColor(Qt::red);
				signalScene->addLine(lastTime, drawingIndex*height+space+BOX_SPACE, time, drawingIndex*height+space+BOX_SPACE, sigPen);
			}
			lastValue = value.value();
			lastTime = time;
			drawn = true;
		}
	}

	if(drawn) {
		// box for the signal
		signalScene->addRect(0, drawingIndex*height+BOX_SPACE, lastTime, height-BOX_SPACE*2, QPen(Qt::white));

		// add it to the list
		signalAreas[signal_name]=QRect(0, drawingIndex*height+BOX_SPACE, lastTime, height-BOX_SPACE*2);

		// name of the signal bus:
		text = signalScene->addSimpleText(signal_name);
		text->setPos(recentZeroTime+space, drawingIndex*height+BOX_SPACE);
		text->setBrush(QColor(Qt::white));

		drawingIndex++;

		return true;
	}

	return false;
}
