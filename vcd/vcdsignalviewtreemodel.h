#ifndef VCDSIGNALVIEWTREEMODEL_H
#define VCDSIGNALVIEWTREEMODEL_H

#include "vcd/vcd_data.hpp"

#include <QAbstractItemModel>

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace vcd {
	class VcdData;
	class Var;
}

class VcdSignalViewTreeItem;

class VcdSignalViewTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit VcdSignalViewTreeModel(vcd::VcdData data, QVector<QString> filter, QObject *parent = 0);
	~VcdSignalViewTreeModel();

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	void showSignals(QVector<QString> filter);

private:
	VcdSignalViewTreeItem *rootItem;
	vcd::VcdData vcdData;
	QList<QVariant> rootData;

};

#endif // VCDSIGNALVIEWTREEMODEL_H
