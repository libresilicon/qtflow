#include "magicdata.h"

namespace magic {
	MagicData::MagicData():
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
	}

	MagicData::MagicData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();
		int stat;

		streamname = filename;
		std::cout << "opening file " << stdfilename << std::endl;
		input.open(stdfilename, std::ios::in);
		std::cout << "opened file" << stdfilename << std::endl;

		lexer = new MagicScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new MagicParser(this);
		//parser->set_debug_level(trace_parsing);
		stat=parser->parse();
		input.close();
	}

	bool MagicData::loadFile(QString filename)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();
		int stat;

		if(filename==QString())
			return false;
		QTextStream(stdout) << filename << '\n';
		trace_scanning = true;

		streamname = filename;

		std::cout << "opening file " << stdfilename << std::endl;
		input.open(stdfilename, std::ios::in);
		std::cout << "opened file" << stdfilename << std::endl;

		if(lexer) delete lexer;
		lexer = new MagicScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		if(parser) delete parser;
		parser = new MagicParser(this);
		//parser->set_debug_level(trace_parsing);
		stat=parser->parse();
		input.close();

		return stat;
	}

	void MagicData::clearParsedElements()
	{
		parsedElements.clear();
	}

	void MagicData::setLayer(std::string *s)
	{
		//title = s;
		QTextStream(stdout) << "setLayer: " << s << '\n';
	}

	class MagicScanner *MagicData::getLexer()
	{
		return lexer;
	}

	void MagicData::addRectangle(int x, int y, int w, int h)
	{
		//QRect objR(x,y,w,h);
		//parsedElements.append(objR);
	}
}
