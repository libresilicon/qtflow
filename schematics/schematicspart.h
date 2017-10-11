#ifndef SCHEMATICSPART_H
#define SCHEMATICSPART_H

#include <QString>

class SchematicsPart
{
public:
	SchematicsPart(QString type, QString id);

	void setPosition(int x, int y);

	QString getType();
	QString getID();
	qreal x();
	qreal y();

private:
	QString m_type;
	QString m_id;
	qreal m_x;
	qreal m_y;
};

#endif // SCHEMATICSPART_H
