#include "schematicsdata.h"

namespace schematics {
	SchematicsData::SchematicsData() :
		lexer(new SchematicsScanner())
	{

	}

	SchematicsScanner *SchematicsData::getLexer()
	{
		return lexer;
	}
}
