#ifndef SCHEMATICSDATA_H
#define SCHEMATICSDATA_H

#include "schematicsscanner.h"

namespace schematics {
	class SchematicsData
	{
	public:
		SchematicsData();
		class SchematicsScanner *getLexer();

	private:
		SchematicsScanner *lexer;
	};
}

#endif // SCHEMATICSDATA_H
