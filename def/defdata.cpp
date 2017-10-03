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
		distanceMicrons(false)
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


	void DEFData::addUsedModuleNames(std::string *module, std::string *name)
	{
		recent_module.module_name = QString::fromStdString(*module);
		recent_module.instance_name = QString::fromStdString(*name);
	}

	void DEFData::addUsedModulePlacement(double x, double y)
	{
		recent_module.x = x;
		recent_module.y = y;

		setBoundaryRectangle(x, y, x, y);
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

	void DEFData::setBoundaryRectangle(int x1, int y1, int x2, int y2)
	{
		if(m_BBLowerX>x1) m_BBLowerX = x1;
		if(m_BBLowerX>x2) m_BBLowerX = x2;

		if(m_BBLowerY>y1) m_BBLowerY = y1;
		if(m_BBLowerY>y2) m_BBLowerY = y2;

		if(m_BBUpperX<x1) m_BBUpperX = x1;
		if(m_BBUpperX<x2) m_BBUpperX = x2;

		if(m_BBUpperY<y1) m_BBUpperY = y1;
		if(m_BBUpperY<y2) m_BBUpperY = y2;
	}
}
