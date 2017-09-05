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
	QPointF pt = mapToScene(e->pos());
	signalScene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));
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
		int idx = 0;
		foreach(QString s, signalViewFilter) {
			drawSignal(s,idx);
			idx++;
		}
	}
}

void VcdSignalView::drawSignal(QString signal_name, int idx)
{
	int height = this->height();
	height /= signalViewFilter.count();
	height /= 2;
	int space = height/2;

	QPen green(Qt::green);

	bool lastHigh = false;
	int lastTime = 0;
	int time;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
		if(mapIdName.contains(signal_name)) {
			if(value.var_id()==mapIdName[signal_name]) {
				time = this->width()*(value.time()-lowest_time)/(highest_time-lowest_time);
				signalScene->addLine(time, idx*height+space, time, (idx+1)*height+space, green);
				if(lastHigh) {
					signalScene->addLine(lastTime, (idx+1)*height+space, time, (idx+1)*height+space , green);
				} else {
					signalScene->addLine(lastTime, idx*height+space, time, idx*height+space, green);
				}
				lastHigh = (value.value()==vcd::LogicValue::ONE);
				lastTime = time;
			}
		}
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
