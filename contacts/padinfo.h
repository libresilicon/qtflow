#ifndef PADINFO_H
#define PADINFO_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QFileInfo>
#include <QTextStream>

class PadInfo
{
public:
	PadInfo(QString padpath);
	void setPadSide(QString side, QStringList pads);
	void setPadCell(QString pad, QString cell);
	void sync();

	QStringList getPadSide(QString side);
	QString getPadCell(QString pad);

private:
	QMap<QString,QStringList> m_sides;
	QMap<QString,QString> m_padCellMapping;
	QString m_filePath;
};

#endif // PADINFO_H
