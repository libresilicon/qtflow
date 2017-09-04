#include <QStringList>

#include "vcdtreeitem.h"

VcdTreeItem::VcdTreeItem(const QList<QVariant> &data, VcdTreeItem *parent)
{
	m_parentItem = parent;
	m_itemData = data;
}

VcdTreeItem::~VcdTreeItem()
{
	qDeleteAll(m_childItems);
}

VcdTreeItem * VcdTreeItem::appendChild(QString key)
{
	QList<QVariant> data;

	if(m_childItemMap.contains(key)) {
		return m_childItemMap[key];
	} else {
		data << key;
		m_childItemMap[key] = new VcdTreeItem(data,this);
		m_childItems.append(m_childItemMap[key]);
		return m_childItemMap[key];
	}
}

VcdTreeItem *VcdTreeItem::child(int row)
{
	return m_childItems.value(row);
}

int VcdTreeItem::childCount() const
{
	return m_childItems.count();
}

int VcdTreeItem::columnCount() const
{
	return m_itemData.count();
}

QVariant VcdTreeItem::data(int column) const
{
	return m_itemData.value(column);
}

VcdTreeItem *VcdTreeItem::parentItem()
{
	return m_parentItem;
}

int VcdTreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<VcdTreeItem*>(this));

	return 0;
}
