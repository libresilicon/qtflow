#include "vcdlistmodel.h"

#include <QDebug>
#include <QMimeData>

VcdListModel::VcdListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

VcdListModel::~VcdListModel()
{
}

void VcdListModel::reset()
{
    _signals.clear();
}

QString VcdListModel::LogicValue2String(vcd::LogicValue l)
{
	QString ret;
	if(l==vcd::LogicValue::ONE)
		ret = "ONE";
	if(l==vcd::LogicValue::ZERO)
		ret = "ZERO";
	if(l==vcd::LogicValue::UNKOWN)
		ret = "UNKOWN";
	if(l==vcd::LogicValue::HIGHZ)
		ret = "HIGHZ";
	return ret;
}

void VcdListModel::set_vcd_data(vcd::VcdData d)
{
	foreach(vcd::TimeValue v, d.time_values()) {
		QTextStream(stdout)
			<< "\t Time: "
			<< v.time()
			<< "\t Value: "
			<< LogicValue2String(v.value())
			<< "\t Var ID: "
			<< v.var_id()
			<< "\n";
	}
}

int VcdListModel::rowCount(const QModelIndex&) const
{
    return _signals.size();
}

QVariant VcdListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _signals.size())
        return QVariant();

    if (role == Qt::TextAlignmentRole)
        return QVariant(Qt::AlignVCenter | Qt::AlignRight);

    if (role == Qt::DisplayRole)
        return _signals.at(index.row()).second;

    return QVariant();
}

Qt::ItemFlags VcdListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    return Qt::ItemIsDragEnabled | QAbstractItemModel::flags(index);
}
