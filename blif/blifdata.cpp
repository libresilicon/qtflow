#include "blifdata.h"
#include "blifscanner.h"

#include <QDebug>

namespace blif {
	BLIFModuleInfo::BLIFModuleInfo() {}

	BLIFData::BLIFData(QString filename) :
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

		lexer = new BLIFScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new BLIFParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	void BLIFData::setAmountComponents(int i)
	{
		amountComponents = i;
	}


	void BLIFData::addUsedModuleNames(std::string *instance_name, std::string *macro_name)
	{
		recent_module.macro_name = QString::fromStdString(*macro_name);
		recent_module.instance_name = QString::fromStdString(*instance_name);
	}

	void BLIFData::addUsedModulePlacement(double x, double y)
	{
		recent_module.x = x;
		recent_module.y = y;
	}

	BLIFScanner *BLIFData::getLexer()
	{
		return lexer;
	}

	void BLIFData::setDistanceUnitMicrons(int t)
	{
		distanceMicrons = true;
		distanceMicronsValue = t;
	}

	int BLIFData::getLowerX()
	{
		return m_BBLowerX;
	}

	int BLIFData::getLowerY()
	{
		return m_BBLowerY;
	}

	int BLIFData::getUpperX()
	{
		return m_BBUpperX;
	}

	int BLIFData::getUpperY()
	{
		return m_BBUpperY;
	}

	void BLIFData::setDieArea(int x1, int x2, int y1, int y2)
	{
		m_BBLowerX = x1;
		m_BBUpperX = x2;
		m_BBLowerY = y1;
		m_BBUpperY = y2;
	}

	int BLIFData::getDistanceUnit()
	{
		return distanceMicronsValue;
	}
}
