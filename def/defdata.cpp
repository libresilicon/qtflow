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
}
