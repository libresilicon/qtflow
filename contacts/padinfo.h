#ifndef PADINFO_H
#define PADINFO_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

class PadInfo
{
public:
	PadInfo(QString padpath);
	void setPadCell(QString pad, QString cell);
	void setPadName(QString pad, QString name);
	void setSideLenth(qreal);
	void sync();

	QStringList getPadList();
	QString getPadCell(QString pad);
	QString getPadName(QString pad);
	qreal getSideLenth();

private:
	QMap<QString,QStringList> m_sides;
	QMap<QString,QString> m_padCellMapping;
	QMap<QString,QString> m_padNameMapping;
	QString m_filePath;
	qreal m_sideLength;
};

#endif // PADINFO_H
