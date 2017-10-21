#ifndef GDSCELL_H
#define GDSCELL_H

#include <QString>
#include <QPointF>
#include <QVector>

class GDSCell
{
public:
	GDSCell(QString name);
	QString getName();

private:
	QVector<QPointF> m_path;
	QString m_name;
};

#endif // GDSCELL_H
