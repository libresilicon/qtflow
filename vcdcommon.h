#ifndef H_VCD_COMMON
#define H_VCD_COMMON

#include <QString>
#include <QHash>
#include <QList>
#include <QVector>
#include <QPair>

struct vcd_scopes_t
{
	QString name;
	QHash<int, QString> alias;
	QList<vcd_scopes_t> scopes;
};

typedef QVector<QPair<int, int>> vcd_changes_t;

struct vcd_t
{
	QString date;
	QString version;
	int timescale;
	vcd_scopes_t scopes;
	QHash<int, vcd_changes_t> changes;
};

#endif
