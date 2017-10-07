#include "schematicsdata.h"
#include "schematicsscanner.h"

namespace schematics {
	SchematicsData::SchematicsData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false),
		m_BBLowerX(0),
		m_BBLowerY(0),
		m_BBUpperX(0),
		m_BBUpperY(0)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new SchematicsScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new SchematicsParser(this);
		//parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	SchematicsScanner *SchematicsData::getLexer()
	{
		return lexer;
	}
}
