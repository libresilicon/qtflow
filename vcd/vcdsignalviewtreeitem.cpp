#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include <QTextStream>

#include "vcd/vcd_data.hpp"

#include "vcdsignalviewtreeitem.h"

VcdSignalViewTreeItem::VcdSignalViewTreeItem(const QList<QVariant> &data, VcdSignalViewTreeItem *parent)
{
	m_parentItem = parent;
	m_itemData = data;
}

VcdSignalViewTreeItem::~VcdSignalViewTreeItem()
{
	qDeleteAll(m_childItems);
}

void VcdSignalViewTreeItem::appendChildItem(VcdSignalViewTreeItem *m)
{
	m_childItems.append(m);
}

void VcdSignalViewTreeItem::appendChild(vcd::Var var)
{
	QString displayName;
	foreach(std::string s, var.hierarchical_name()) {
		displayName += '/';
		displayName += QString::fromStdString(s);
	}

	QList<QVariant> data;
	data << displayName;
	VcdSignalViewTreeItem* signal = new VcdSignalViewTreeItem(data,this);
	m_childItems.append(signal);

	VcdSignalViewTreeItem* wire;
	if(isBus(QString::fromStdString(var.name()))) {
		foreach(QString bit, getBus(QString::fromStdString(var.name()))) {
			QList<QVariant> data;
			data << bit;
			wire = new VcdSignalViewTreeItem(data,signal);
			signal->appendChildItem(wire);
		}
	}
}

bool VcdSignalViewTreeItem::isBus(QString s)
{
	if(s.contains('[') && s.contains(']')) return true;
	return false;
}

QVector<QString> VcdSignalViewTreeItem::getBus(QString s)
{
	int lowerAddress = 0;
	int higherAddress = 0;
	int a,b;
	QVector<QString> ret;
	QStringList spltst;
	QRegExp rx("\\[(.*)\\]");
	rx.setMinimal(true);
	qInfo() << "Getting name: " << s;

	int pos = rx.indexIn(s);
	if (pos > -1) {
		foreach(QString st, rx.capturedTexts()) {
			if(st.contains('[')||st.contains(']'))
				continue;
			if(st.contains(':')) {
				spltst = st.split(':');
				a = spltst.at(0).toInt();
				b = spltst.at(1).toInt();
				lowerAddress = (a<b)?a:b;
				higherAddress = (a>b)?a:b;
			}
		}
		for(int i=lowerAddress;i<=higherAddress;i++) {
			ret.append(QString("foo[")+i+"]");
		}
	}

	return ret;
}

VcdSignalViewTreeItem *VcdSignalViewTreeItem::child(int row)
{
	return m_childItems.value(row);
}

int VcdSignalViewTreeItem::childCount() const
{
	return m_childItems.count();
}

int VcdSignalViewTreeItem::columnCount() const
{
	return m_itemData.count();
}

QVariant VcdSignalViewTreeItem::data(int column) const
{
	return m_itemData.value(column);
}

VcdSignalViewTreeItem *VcdSignalViewTreeItem::parentItem()
{
	return m_parentItem;
}

int VcdSignalViewTreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<VcdSignalViewTreeItem*>(this));

	return 0;
}
