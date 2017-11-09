#ifndef DEFROUTEINFO_H
#define DEFROUTEINFO_H

#include <QPointF>
#include <QVector>
#include <QString>

class DEFRouteInfo
{
public:
	DEFRouteInfo();
	void addPoint(qreal x, qreal y);
	void setLayer(QString s);
	void setVia(QString s);

	QVector<QPointF> getPoints();
	QString getLayer();
	QString getViaName();

private:
	QVector<QPointF> m_pointList;
	QString m_layer;
	QString m_viaName;
};

#endif // DEFROUTEINFO_H
