#include "blifdatacomponentinfo.h"

BLIFDataComponentInfo::BLIFDataComponentInfo()
{

}

void BLIFDataComponentInfo::setName(QString s)
{
	m_name = s;
}

QString BLIFDataComponentInfo::getName()
{
	return m_name;
}

QStringList BLIFDataComponentInfo::getPins()
{
	return m_signalMapping.keys();
}

QString BLIFDataComponentInfo::getSignal(QString s)
{
	return m_signalMapping[s];
}

void BLIFDataComponentInfo::setSignalMapping(QString s)
{
	QString pin;
	QString signal;
	QStringList sigMapping = s.split('=');
	if(sigMapping.count()>1) {
		pin = sigMapping[0];
		signal = sigMapping[1];
		signal.remove('\\');
		m_signalMapping[pin]=signal;
	}
}
