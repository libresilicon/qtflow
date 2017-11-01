#ifndef LEFLAYERINFO_H
#define LEFLAYERINFO_H

#include <QString>

class LEFLayerInfo
{
public:
	LEFLayerInfo();
	void setName(QString s);
	void setType(QString s);
	void setPitch(double i);

	QString getName();
	QString getType();
	double getPitch();

private:
	QString m_name;
	QString m_type;
	double m_pitch;
};

#endif // LEFLAYERINFO_H
