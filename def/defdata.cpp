#include "defdata.h"
#include "defscanner.h"

namespace def {
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

	void DEFData::setLayer(std::string *s)
	{
		recentTitle = QString::fromStdString(*s);
	}

	void DEFData::setTechnology(std::string *s)
	{
		technology = QString::fromStdString(*s);
	}
	
	void DEFData::setAmountComponents(int i)
	{
		
	}

	QString DEFData::getTechnology()
	{
		return technology;
	}

	DEFScanner *DEFData::getLexer()
	{
		return lexer;
	}

	layer_rects_t DEFData::getBoxes()
	{
		return parsedBoxes;
	}

	layer_rects_t DEFData::getRectangles()
	{
		return parsedRectangles;
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
