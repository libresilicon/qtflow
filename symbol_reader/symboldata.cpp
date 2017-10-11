#include "symbol_parser/symbol_parser.h"
#include "symbolscanner.h"
#include "symboldata.h"

namespace symbol {
	SymbolData::SymbolData(QString filename) :
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

		lexer = new SymbolScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new SymbolParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	SymbolScanner *SymbolData::getLexer()
	{
		return lexer;
	}

	QStringList SymbolData::getPlanes()
	{
		return planeList;
	}

	QStringList SymbolData::getTypeNames()
	{
		return typeList.keys();
	}

	QStringList SymbolData::getStyleNames()
	{
		return styleList.keys();
	}

	void SymbolData::addPlane(std::string s)
	{
		planeList.append(QString::fromStdString(s));
	}

	void SymbolData::addType(std::string s1, std::string s2)
	{
		QString typeName = QString::fromStdString(s1);
		QString value = QString::fromStdString(s2);
		typeList[typeName].append(value);
	}

	QStringList SymbolData::getType(QString s)
	{
		return typeList[s];
	}

	QMap<QString,QVector<int>> SymbolData::getStyle(QString s)
	{
		return styleList[s];
	}

	void SymbolData::setRecentStyle(std::string s)
	{
		recentStyle = QString::fromStdString(s);
	}

	void SymbolData::addToRecentStyleMember(int i)
	{
		styleList[recentStyle][recentStyleMember].append(i);
	}

	void SymbolData::setRecentStyleMember(std::string s)
	{
		recentStyleMember = QString::fromStdString(s);
	}
}
