#ifndef VCDVcdTreeModel_H
#define VCDVcdTreeModel_H

#include <QAbstractItemModel>

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class VcdTreeItem;

class VcdTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit VcdTreeModel(const QString &data, QObject *parent = 0);
	~VcdTreeModel();

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
					  const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
	void setupModelData(const QStringList &lines, VcdTreeItem *parent);

	VcdTreeItem *rootItem;
};

#endif // VCDVcdTreeModel_H
