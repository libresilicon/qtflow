#ifndef VCDSIGNALTREEITEM_H
#define VCDSIGNALTREEITEM_H

#include <QList>
#include <QVariant>

namespace vcd {
	class Var;
}

class VcdSignalTreeItem
{
public:
	explicit VcdSignalTreeItem(const QList<QVariant> &data, VcdSignalTreeItem *parentItem = 0);
	~VcdSignalTreeItem();

	void appendChild(vcd::Var var);

	VcdSignalTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	VcdSignalTreeItem *parentItem();

private:
	QList<VcdSignalTreeItem*> m_childItems;
	QMap<QString,VcdSignalTreeItem*> m_childItemMap;
	QList<QVariant> m_itemData;
	VcdSignalTreeItem *m_parentItem;
};

#endif // VCDSIGNALTREEITEM_H
