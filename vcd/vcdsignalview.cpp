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
	qreal highestWidth = 0;
	foreach(VcdViewGraphicsItem *m, m_signals) {
		if(m->width() > highestWidth) highestWidth = m->width();
	}
	scale(width()/highestWidth,1.0);
}

void VcdSignalView::onRemoveSignal(VcdViewGraphicsItem* m)
{
	qreal lastHeight = 0;

	for(int i; i<m_signals.count();i++) {
		if(m_signals.at(i)==m) {
			m_signals.remove(i);
			break;
		}
	}
	signalScene->removeItem(m);

	foreach(VcdViewGraphicsItem *m, m_signals) {
		m->setPos(0,lastHeight);
		lastHeight+=m->height()+20;
	}

	update();
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
	VcdViewGraphicsItemBus *bus;
	VcdViewGraphicsItemSignal *signal;
	int i;

	foreach(VcdViewGraphicsItem *m, m_signals) {
		if(m->getLongName()==s) return;
	}

	foreach(vcd::Var var, vcd_data.vars()) {
		if(longSignalID(var.hierarchical_name())==s) {
			if(var.width()>1) {
				bus = new VcdViewGraphicsItemBus(var,vcd_data.time_bus_values());
				connect(bus,SIGNAL(deleteInstance(VcdViewGraphicsItem*)),this,SLOT(onRemoveSignal(VcdViewGraphicsItem*)));
				signalScene->addItem(bus);
				m_signals.append(bus);
			} else {
				signal = new VcdViewGraphicsItemSignal(var,vcd_data.time_values());
				connect(signal,SIGNAL(deleteInstance(VcdViewGraphicsItem*)),this,SLOT(onRemoveSignal(VcdViewGraphicsItem*)));
				signalScene->addItem(signal);
				m_signals.append(signal);
			}
		}
	}

	qreal lastHeight = 0;
	foreach(VcdViewGraphicsItem *m, m_signals) {
		m->setPos(0,lastHeight);
		lastHeight+=m->height()+20;
	}

	update();
}

void VcdSignalView::setVCD(vcd::VcdData d)
{
	QStringList toReAdd;
	vcd_data = d;

	m_highest_time = 0;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
			if(value.time()>m_highest_time) m_highest_time = value.time();
	}
	m_lowest_time = m_highest_time;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
			if(value.time()<m_lowest_time) m_lowest_time = value.time();
	}

	foreach(VcdViewGraphicsItem *m, m_signals) {
		toReAdd << m->getLongName();
		signalScene->removeItem(m);
	}
	m_signals.clear();

	foreach(QString s, toReAdd) {
		append(s);
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

