#ifndef LEFDATA_H
#define LEFDATA_H

#include "lefscanner.h"

namespace lef {
	class LEFData
	{
	public:
		LEFData();
		LEFScanner *getLexer();
	private:
		LEFScanner *lexer;
	};
}

#endif // LEFDATA_H
