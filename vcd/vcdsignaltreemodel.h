#ifndef VCDSIGNALTREEMODEL_H
#define VCDSIGNALTREEMODEL_H

#include "vcd/vcd_data.hpp"

#include <QAbstractItemModel>

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace vcd {
	class VcdData;
	class Var;
}

class VcdTreeItem;

class VcdSignalTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit VcdSignalTreeModel(const vcd::VcdData &data, QObject *parent = 0);
	~VcdSignalTreeModel();

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	void showSignals(QStringList);

private:
	VcdTreeItem *rootItem;
	vcd::VcdData vcdData;

};

#endif // VCDSIGNALTREEMODEL_H
