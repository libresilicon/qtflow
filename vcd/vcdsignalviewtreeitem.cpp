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

	if(isBus(QString::fromStdString(var.name()))) {
		foreach(QString bit, getBus(QString::fromStdString(var.name()))) {
			qInfo() << "Getting bit: " <<  bit;
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
	QVector<QString> ret;
	QRegExp rx("\\[*0-9+:\\]");
	rx.setMinimal(true);
	qInfo() << "Getting name: " << s;

	int pos = rx.indexIn(s);
	if (pos > -1) {
		qInfo() << "Captured texts: " << rx.capturedTexts();
		qInfo() << "timestamp cap: " <<rx.cap(0);
		qInfo() << "timestamp cap: " <<rx.cap(1);
		qInfo() << "timestamp cap: " <<rx.cap(2);
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
