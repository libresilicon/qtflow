#ifndef VCDLISTMODEL_H
#define VCDLISTMODEL_H

#include <QAbstractListModel>
#include <QItemDelegate>
#include <QList>
#include <QPair>
#include <QString>

#include "vcd/vcd_data.hpp"

class VcdListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit VcdListModel(QObject *parent = 0);
	~VcdListModel();

	void reset();

	QString LogicValue2String(vcd::LogicValue l);
	void set_vcd_data(vcd::VcdData);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
	void signalsChanged();

private:
	QList<QPair<vcd::LogicValue, QString>> _signals;
};

class VcdListDelegate : public QItemDelegate
{
    Q_OBJECT
	public:
	VcdListDelegate(QObject *parent) : QItemDelegate(parent) {}

	QSize sizeHint (const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		 QSize result = QItemDelegate::sizeHint(option, index);
		 return result;
	}
};

#endif // VCDLISTMODEL_H
