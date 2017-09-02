#include "vcdata.h"

namespace vcd {
	VCData::VCData(QString filename) :
		lexer(new VCDScanner()),
		trace_scanning(false),
		trace_parsing(false),
		rootSet(false)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();
		std::cout << "opening file: " << stdfilename << std::endl;

		trace_scanning = true;

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new VCDScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new VCDParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	VCDScanner *VCData::getLexer()
	{
		return lexer;
	}

	void VCData::setDate(std::string *s)
	{
		parsedVcd.date = QString::fromStdString(*s);
	}

	void VCData::setVersion(std::string *s)
	{
		parsedVcd.version = QString::fromStdString(*s);
	}

	void VCData::addTimescale(std::string *s)
	{
		parsedVcd.timescale = QString::fromStdString(*s).toInt();
	}

	void VCData::addScope(std::string *s)
	{
		if(rootSet) {
			vcd_scopes_t scope;
			scope.name = QString::fromStdString(*s);
			parsedVcd.scope.scopes.append(scope);
		} else {
			parsedVcd.scope.name = QString::fromStdString(*s);
		}
	}

	vcd_t VCData::getVCD()
	{
		return parsedVcd;
	}

}
