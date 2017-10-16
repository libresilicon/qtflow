#include "vcdsignalview.h"

VcdSignalView::VcdSignalView(QWidget *parent) :
	QGraphicsView(parent),
	signalScene(new QGraphicsScene(this)),
	moveDragLastX(0),
	m_lowest_time(0),
	m_highest_time(0)
{
	setAcceptDrops(true);
	signalScene->setBackgroundBrush(Qt::black);
	setScene(signalScene);
}

void VcdSignalView::onZoomIn()
{
	scale(1.1,1.0);
}

void VcdSignalView::onZoomOut()
{
	scale(0.9,1.0);
}

void VcdSignalView::onZoomFitWidth()
{
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
}

QString VcdSignalView::longSignalID(std::vector<std::string> arr)
{
	QString ret;
	foreach(std::string s, arr) {
		ret+='/';
		ret+=QString::fromStdString(s);
	}
	return ret;
}

void VcdSignalView::append(QString s)
{
	VcdViewGraphicsItem *nm;
	VcdViewGraphicsItemBus *bus;
	VcdViewGraphicsItemSignal *signal;
	int i;

	foreach(vcd::Var var, vcd_data.vars()) {
		if(longSignalID(var.hierarchical_name())==s) {
			if(var.width()>1) {
				bus = new VcdViewGraphicsItemBus(var,vcd_data.time_bus_values());
				signalScene->addItem(bus);
				m_signals.append(bus);
			} else {
				signal = new VcdViewGraphicsItemSignal(var,vcd_data.time_values());
				signalScene->addItem(signal);
				m_signals.append(signal);
			}
		}
	}

	i = 0;
	foreach(VcdViewGraphicsItem *m, m_signals) {
		m->setPos(0,i*20);
		i++;
	}

	update();
}

void VcdSignalView::setVCD(vcd::VcdData d)
{
	vcd_data = d;

	m_highest_time = 0;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
			if(value.time()>m_highest_time) m_highest_time = value.time();
	}
	m_lowest_time = m_highest_time;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
			if(value.time()<m_lowest_time) m_lowest_time = value.time();
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

