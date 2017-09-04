#ifndef VCDSIGNALVIEWTREEITEM_H
#define VCDSIGNALVIEWTREEITEM_H

#include <QList>
#include <QVariant>

namespace vcd {
	class Var;
}

class VcdSignalViewTreeItem
{
public:
	explicit VcdSignalViewTreeItem(const QList<QVariant> &data, VcdSignalViewTreeItem *parentItem = 0);
	~VcdSignalViewTreeItem();

	void appendChild(vcd::Var var);
	void appendChildItem(VcdSignalViewTreeItem *m);

	VcdSignalViewTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	VcdSignalViewTreeItem *parentItem();

	bool isBus(QString);
	QVector<QString> getBus(QString s);

private:
	QList<VcdSignalViewTreeItem*> m_childItems;
	QList<QVariant> m_itemData;
	VcdSignalViewTreeItem *m_parentItem;
};

#endif // VCDSIGNALVIEWTREEITEM_H
