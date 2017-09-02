#include "vcdata.h"
#include "vcdscanner.h"
#include "vcd_parser.h"

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

		parser = new VCDParser(lexer, this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}
}
