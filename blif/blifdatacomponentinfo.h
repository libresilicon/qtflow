#ifndef BLIFDATACOMPONENTINFO_H
#define BLIFDATACOMPONENTINFO_H

#include <QString>
#include <QStringList>
#include <QMap>

class BLIFDataComponentInfo
{
public:
	BLIFDataComponentInfo();
	void setName(QString s);
	void setSignalMapping(QString s);
	QString getName();
	QStringList getPins();
	QString getSignal(QString s);

private:
	QString m_name;
	QMap<QString,QString> m_signalMapping;
};

#endif // BLIFDATACOMPONENTINFO_H
