#include "vcdsignalview.h"

#include <QDebug>

VcdSignalView::VcdSignalView(QWidget *parent) :
	QGraphicsView(parent),
	signalScene(new QGraphicsScene(this))
{
	signalScene->setBackgroundBrush(Qt::black);
	setScene(signalScene);
}


void VcdSignalView::mousePressEvent(QMouseEvent * e)
{
	double rad = 10;
	//QPointF pt = mapToScene(e->pos());
	//signalScene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));
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
		signalScene->clear();
		drawTimeScale();
		int idx = 0;
		foreach(QString s, signalViewFilter) {
			drawSignal(s,idx);
			drawSignalBus(s,idx);
			idx++;
		}
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
				time = this->width()*(bus.time()-lowest_time)/(highest_time-lowest_time);
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
		}
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
	QPen white(Qt::white);

	vcd::LogicValue lastValue;
	int lastTime = 0;
	int time;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
		if(value.var_id()==mapIdName[signal_name]) {
			time = this->width()*(value.time()-lowest_time)/(highest_time-lowest_time);
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
		signalScene->addLine(0, idx*height, this->width(), idx*height, white);
		signalScene->addLine(0, (idx+1)*height, this->width(), (idx+1)*height, white);
	}

	//QList<int> sig = list->getSignals();
	//for (int i = 0; i < sig.size(); ++i)
	//{
		/*vcd_changes_t::iterator it;
	vcd_changes_t changes = tree->getValues(sig.at(i));

        int high_ = 1;
        int long_ = 0;
        for (it = changes.begin(); it != changes.end(); ++it)
        {
            if (it->first > long_)
                long_ = it->first;
            if (it->second > high_)
                high_ = it->second;
        }

        int time = 0;
        int state = 0;
                int h = i * ui->dockWidgetContents->height();
                double hs = ui->dockWidgetContents->height() / high_;
                double ws = tree->vcd().timescale * ui->dockWidgetContents->height() / 1000000;
                int margin = ui->dockWidgetContents->width() / 10;
        QPen green(Qt::green);
        for (it = changes.begin(); it != changes.end(); ++it)
        {
            // two state
            if (high_ < 2)
            {
                scene->addLine(time * ws, h + state * hs, it->first * ws, h + state * hs, green);
                scene->addLine(it->first * ws, h + state * hs, it->first * ws, h + it->second * hs, green);
            }
            // more than two states
            else
            {
                scene->addLine(time * ws + margin, h + margin, it->first * ws - margin, h + margin, green);
                                scene->addLine(time * ws - margin, h + margin, time * ws + margin, h + ui->dockWidgetContents->height() - margin, green);
                                scene->addLine(time * ws + margin, h + ui->dockWidgetContents->height() - margin, it->first * ws - margin, h + ui->dockWidgetContents->height() - margin, green);
                                scene->addLine(time * ws - margin, h + ui->dockWidgetContents->height() - margin, time * ws + margin, h + margin, green);

                QGraphicsTextItem* ann = new QGraphicsTextItem;
                ann->setPos(time * ws, h);
                ann->setDefaultTextColor(QColor(Qt::white));
                ann->setPlainText(QString::number(it->second));

                scene->addItem(ann);
            }

            time = it->first;
            state = it->second;
                }*/
        //}
}
