#include "padinfo.h"

PadInfo::PadInfo(QString padpath) :
	m_filePath(padpath)
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
						s = line[1];
						for(int i=2; i<line.count(); i++) {
							m_sides[s].append(line[i]);
						}
					} else if(line[0]=="pad") {
						padName = line[1];
						cellName = line[3];
						m_padCellMapping[padName]=cellName;
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
		foreach(QString k, m_sides.keys()) {
			out << "side " << k;
			foreach(QString p, m_sides[k]) {
				out << " " << p;
			}
			out << endl;
			out << endl;
		}
		foreach(QString k, m_padCellMapping.keys()) {
			out << "pad " << k;
			out << " cell " << m_padCellMapping[k];
			out << endl;
		}
		outputFile.close();
	}
}

void PadInfo::setPadSide(QString side, QStringList pads)
{
	m_sides[side] = pads;
}

QStringList PadInfo::getPadSide(QString side)
{
	return m_sides[side];
}

void PadInfo::setPadCell(QString pad, QString cell)
{
	m_padCellMapping[pad]=cell;
}

QString PadInfo::getPadCell(QString pad)
{
	return m_padCellMapping[pad];
}
