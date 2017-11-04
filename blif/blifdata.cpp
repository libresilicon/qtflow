#include "blifdata.h"

namespace blif {
	BLIFData::BLIFData(QString filename)
	{
		QStringList lineList;
		QString line;
		QFile inputFile(filename);
		if (inputFile.open(QIODevice::ReadOnly)) {
			QTextStream in(&inputFile);
			while (!in.atEnd()) {
				line = in.readLine();
				lineList = line.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);
				if(lineList.count()>1) {
					// keywords
					switch(tokenize(lineList[0])) {
						case MODEL:
							m_modelName = lineList[1];
							break;
						case INPUTS:
							registerInputPins(lineList);
						break;
						case OUTPUTS:
							registerOutputPins(lineList);
						break;
						case NAMES:
						break;
						case GATE:
						case SUB_COMPONENT:
							registerComponent(lineList);
						break;
						case END:
						break;
					}
				}
			}
			inputFile.close();
		}
	}

	blif_keyword BLIFData::tokenize(QString s)
	{
		if(s==".model") return MODEL;
		if(s==".inputs") return INPUTS;
		if(s==".outputs") return OUTPUTS;
		if(s==".names") return NAMES;
		if(s==".gate") return GATE;
		if(s==".subckt") return SUB_COMPONENT;
		if(s==".end") return END;
		return UNKNOWN;
	}

	void BLIFData::registerInputPins(QStringList lineList)
	{
		for(int i=1;i<lineList.count();i++) {
			m_inputList.append(lineList.at(i));
		}
	}

	void BLIFData::registerOutputPins(QStringList lineList)
	{
		for(int i=1;i<lineList.count();i++) {
			m_outputList.append(lineList.at(i));
		}
	}

	void BLIFData::registerComponent(QStringList lineList)
	{
		BLIFDataComponentInfo component;
		component.setName(lineList[1]);
		for(int i=2; i<lineList.count(); i++) {
			component.setSignalMapping(lineList[i]);
		}
		m_components.append(component);
	}

	QVector<BLIFDataComponentInfo> BLIFData::getComponents()
	{
		return m_components;
	}

	QStringList BLIFData::getPadPinsInput()
	{
		return m_inputList;
	}

	QStringList BLIFData::getPadPinsOutput()
	{
		return m_outputList;
	}

	QStringList BLIFData::getPadPinsPower()
	{
		QStringList ret;
		ret << "vdd";
		return ret;
	}

	QStringList BLIFData::getPadPinsGround()
	{
		QStringList ret;
		ret << "gnd";
		return ret;
	}

}
