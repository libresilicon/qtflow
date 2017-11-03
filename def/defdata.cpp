#include "defdata.h"
#include "defscanner.h"

namespace def {
	DEFData::DEFData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false),
		distanceMicrons(false),
		m_BBLowerX(0),
		m_BBLowerY(0),
		m_BBUpperX(0),
		m_BBUpperY(0)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new DEFScanner(&input, &std::cout);
		//lexer->set_debug(trace_scanning);

		parser = new DEFParser(this);
		//parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	void DEFData::setAmountComponents(int i)
	{
		amountComponents = i;
	}


	void DEFData::addUsedModuleNames(std::string instance_name, std::string macro_name)
	{
		m_recentModule.macro_name = QString::fromStdString(macro_name);
		m_recentModule.instance_name = QString::fromStdString(instance_name);
	}

	void DEFData::addUsedModulePlacement(double x, double y)
	{
		m_recentModule.x = x;
		m_recentModule.y = y;
	}

	void DEFData::addUsedModule()
	{
		m_parsedModules.append(m_recentModule);
	}

	void DEFData::addUsedPin()
	{
		m_parsedPins.append(m_recentPin);
	}

	DEFScanner *DEFData::getLexer()
	{
		return lexer;
	}

	pins_t DEFData::getPins()
	{
		return m_parsedPins;
	}

	mods_t DEFData::getModules()
	{
		return m_parsedModules;
	}

	void DEFData::setDistanceUnitMicrons(int t)
	{
		distanceMicrons = true;
		distanceMicronsValue = t;
	}

	void DEFData::addPin(std::string s)
	{
		m_recentPin = DEFDataPin();
		m_recentPin.m_name = QString::fromStdString(s);
		qDebug() << "Added pin " << m_recentPin.m_name;
	}

	void DEFData::setPinPosition(qreal x, qreal y)
	{
		m_recentPin.m_x = x;
		m_recentPin.m_y = y;
	}

	void DEFData::setPinArea(qreal x1, qreal y1, qreal x2, qreal y2)
	{
		m_recentPin.m_w = x2 - x1;
		m_recentPin.m_h = y2 - y1;
	}

	void DEFData::setPinLayer(std::string s)
	{
		m_recentPin.m_layer = QString::fromStdString(s);
	}

	int DEFData::getLowerX()
	{
		return m_BBLowerX;
	}

	int DEFData::getLowerY()
	{
		return m_BBLowerY;
	}

	int DEFData::getUpperX()
	{
		return m_BBUpperX;
	}

	int DEFData::getUpperY()
	{
		return m_BBUpperY;
	}

	void DEFData::setDieArea(qreal x1, qreal y1, qreal x2, qreal y2)
	{
		m_BBLowerX = x1;
		m_BBUpperX = x2;
		m_BBLowerY = y1;
		m_BBUpperY = y2;
	}

	int DEFData::getDistanceUnit()
	{
		return distanceMicronsValue;
	}
}
