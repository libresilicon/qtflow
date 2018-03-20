#include <QString>
#include <QTemporaryDir>
#include <QDebug>

#include "toml11/toml.hpp"

class TechDataWrapper {
public:
	TechDataWrapper(QString s);
	QStringList getLayers();
private:
	QTemporaryDir tmp;
	QString tempFile;
	toml::Table pr;
};
