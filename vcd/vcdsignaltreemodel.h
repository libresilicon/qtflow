#ifndef VCDSIGNALTREEMODEL_H
#define VCDSIGNALTREEMODEL_H

#include "vcd_reader/vcd_data.hpp"

#include <QAbstractItemModel>

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace vcd {
	class VcdData;
	class Var;
}

class VcdSignalTreeItem;

class VcdSignalTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit VcdSignalTreeModel(vcd::VcdData data, QVector<QString> filter, QObject *parent = 0);
	~VcdSignalTreeModel();

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	Qt::DropActions supportedDropActions() const;

	void showSignals(QVector<QString> filter);

private:
	VcdSignalTreeItem *rootItem;
	vcd::VcdData vcdData;
	QList<QVariant> rootData;

};

#endif // VCDSIGNALTREEMODEL_H
