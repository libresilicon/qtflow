#include "vcd_parser/location.hh"
#include "vcdata_types.h"
#include "vcdata.h"
#include "vcd_parser/vcd_parser.h"
#include "vcdscanner.h"

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

	void newHeader(const Header new_header, std::vector<Var> new_vars, TimeValues new_time_values)
	{
	}

}
