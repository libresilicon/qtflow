#include "tech_parser/tech_parser.h"
#include "techscanner.h"
#include "techdata.h"

namespace tech {
	TechData::TechData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
		planeList.clear();

		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new TechScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new TechParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	TechScanner *TechData::getLexer()
	{
		return lexer;
	}

	QStringList TechData::getPlanes()
	{
		return planeList;
	}

	QStringList TechData::getTypeNames()
	{
		return typeList.keys();
	}

	void TechData::addPlane(std::string s)
	{
		planeList.append(QString::fromStdString(s));
	}

	void TechData::addType(std::string s1, std::string s2)
	{
		QString typeName = QString::fromStdString(s1);
		QString value = QString::fromStdString(s2);
		typeList[typeName].append(value);
	}

	QStringList TechData::getType(QString s)
	{
		return typeList[s];
	}

}
