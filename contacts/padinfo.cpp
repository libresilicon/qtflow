#include "padinfo.h"

PadInfo::PadInfo(QString padpath) :
	m_filePath(padpath),
	m_sideLength(0)
{
	QString s;
	QString padName;
	QString cellName;
	QStringList line;
	if(QFileInfo(padpath).exists()) {
		QFile inputFile(padpath);
		if (inputFile.open(QIODevice::ReadOnly)) {
			QTextStream in(&inputFile);
			while (!in.atEnd()) {
				line = in.readLine().split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
				if(line.count()) {
					if(line[0]=="side") {
						s = (line.count()>1)?line[1]:QString();
						for(int i=2; i<line.count(); i++) {
							m_sides[s].append(line[i]);
						}
					} else if(line[0]=="pad") {
						padName = (line.count()>1)?line[1]:QString();
						if(line.count()>2) {
							s = (line.count()>2)?line[2]:QString();
							if(s=="cell") {
								cellName = (line.count()>3)?line[3]:QString();
								m_padCellMapping[padName]=cellName;
							} else if(s=="name") {
								cellName = (line.count()>3)?line[3]:QString();
								m_padNameMapping[padName]=cellName;
							}
						}
					} else if(line[0]=="var") {
						s = (line.count()>1)?line[1]:QString();
						if(s=="side_length") {
							m_sideLength = (line.count()>2)?line[2].toDouble():1;
						}
					}
				}
			}
			inputFile.close();
		}
	}
}

void PadInfo::sync()
{
	QFile outputFile(m_filePath);
	if (outputFile.open(QIODevice::WriteOnly)) {
		QTextStream out(&outputFile);

		out << "var side_length " << m_sideLength;
		out << endl;

		foreach(QString k, m_padCellMapping.keys()) {
			out << "pad " << k;
			out << " cell " << m_padCellMapping[k];
			out << endl;
		}

		foreach(QString k, m_padNameMapping.keys()) {
			out << "pad " << k;
			out << " name " << m_padNameMapping[k];
			out << endl;
		}

		outputFile.close();
	}
}

void PadInfo::setPadCell(QString pad, QString cell)
{
	m_padCellMapping[pad]=cell;
}

void PadInfo::setPadName(QString pad, QString name)
{
	m_padNameMapping[pad]=name;
}

QString PadInfo::getPadCell(QString pad)
{
	return m_padCellMapping[pad];
}

QStringList PadInfo::getPadList()
{
	return m_padCellMapping.keys();
}

QString PadInfo::getPadName(QString pad)
{
	return (m_padNameMapping[pad]==QString())?QString("n/c"):m_padNameMapping[pad];
}

void PadInfo::setSideLenth(qreal l)
{
	m_sideLength = l;
}

qreal PadInfo::getSideLenth()
{
	return m_sideLength;
}
