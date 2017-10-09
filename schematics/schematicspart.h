#ifndef SCHEMATICSPART_H
#define SCHEMATICSPART_H

#include <QString>

class SchematicsPart
{
public:
	SchematicsPart();

	void setName(QString);
	void setType(QString);
	void setPosition(int x, int y);

	QString getName();
	QString getType();
	qreal x();
	qreal y();

private:
	QString m_name;
	QString m_type;
	qreal m_x;
	qreal m_y;
};

#endif // SCHEMATICSPART_H
