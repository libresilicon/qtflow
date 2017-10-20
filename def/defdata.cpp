#include "defdata.h"
#include "defscanner.h"

#include <QDebug>

namespace def {
	DEFModuleInfo::DEFModuleInfo() {}

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
		lexer->set_debug(trace_scanning);

		parser = new DEFParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	void DEFData::setAmountComponents(int i)
	{
		amountComponents = i;
	}


	void DEFData::addUsedModuleNames(std::string *instance_name, std::string *macro_name)
	{
		recent_module.macro_name = QString::fromStdString(*macro_name);
		recent_module.instance_name = QString::fromStdString(*instance_name);
	}

	void DEFData::addUsedModulePlacement(double x, double y)
	{
		recent_module.x = x;
		recent_module.y = y;
	}

	void DEFData::addUsedModule()
	{
		parsedModules.append(recent_module);
	}

	DEFScanner *DEFData::getLexer()
	{
		return lexer;
	}

	mods_t DEFData::getModules()
	{
		return parsedModules;
	}

	void DEFData::setDistanceUnitMicrons(int t)
	{
		distanceMicrons = true;
		distanceMicronsValue = t;
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

	void DEFData::setDieArea(int x1, int x2, int y1, int y2)
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
