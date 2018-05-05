#ifndef HEADER_TECHDATA_WRAPPER
#define HEADER_TECHDATA_WRAPPER

#include <QString>
#include <QTemporaryDir>
#include <QDebug>
#include <QColor>
#include <QIcon>

#include <QDomDocument>

#include <typeinfo>
#include <iostream>
#include <cassert>

class TechDataWrapper {
public:
	TechDataWrapper(QString s);
	QStringList getLayers();
	QColor getLayerColor(QString);
	QIcon getLayerIcon(QString n);
	QString getLambdaUnit();
	qreal getLambdaValue();
private:
	QDomDocument doc;
};

#endif
