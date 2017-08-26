#include "vcdata.h"

namespace vcd {
	VCData::VCData() :
		lexer(new VCDScanner())
	{

	}

	VCDScanner *VCData::getLexer()
	{
		return lexer;
	}

	void VCData::setDate(std::string *s)
	{}

	void VCData::setVersion(std::string *s)
	{}

	void VCData::addTimescale(std::string *s)
	{}

	void VCData::addScope(std::string *s)
	{}
}
