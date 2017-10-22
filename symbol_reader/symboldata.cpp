#include "symbol_parser/symbol_parser.h"
#include "symbolscanner.h"
#include "symboldata.h"

namespace symbol {
	SymbolData::SymbolData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false),
		recentSymbol(NULL)
	{
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

	void SymbolData::addPart(std::string name, std::string prefix, int x, int y, std::string a, std::string b, int foo, std::string f, std::string font)
	{
		recentSymbol = new SchematicsSymbol(QString::fromStdString(name),QString::fromStdString(prefix));
		partsymbols.append(recentSymbol);
	}

	void SymbolData::addPin(std::string name, int index, int x, int y, int length, std::string orient, int tw, int th, int a, int b, std::string mode)
	{
		recentSymbol->addPin(name, index, x, y, length, orient, tw, th, a, b, mode);
	}

	bool SymbolData::isDefinedSymbol(QString name)
	{
		foreach(SchematicsSymbol *s, partsymbols)
			if(s->getName()==name)
				return true;
		return false;
	}

	SchematicsSymbol* SymbolData::getSymbol(QString name)
	{
		foreach(SchematicsSymbol *s, partsymbols)
			if(s->getName()==name)
				return s;
		return NULL;
	}

	QStringList SymbolData::getSymbolNames()
	{
		QStringList ret;
		foreach(SchematicsSymbol *s, partsymbols)
			ret << s->getName();
		return ret;
	}

	void SymbolData::addRect(int x1, int y1, int x2, int y2)
	{
		if(recentSymbol) {
			recentSymbol->addRect(x1,y1,x2,y2);
		}
	}
}
