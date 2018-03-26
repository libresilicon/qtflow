#ifndef HEADER_TECHDATA_WRAPPER
#define HEADER_TECHDATA_WRAPPER

#include <QString>
#include <QTemporaryDir>
#include <QDebug>
#include <QColor>
#include <QIcon>

#include "toml11/toml.hpp"

class TechDataWrapper {
public:
	TechDataWrapper(QString s);
	QStringList getLayers();
	QColor getLayerColor(QString);
	QIcon getLayerIcon(QString n);
	QString getLambdaUnit();
	qreal getLambdaValue();
private:
	QTemporaryDir tmp;
	QString tempFile;
	toml::Table pr;
};

#endif
