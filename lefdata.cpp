#include "lefdata.h"

namespace lef {
	LEFData::LEFData()
	{
	}
	
	LEFScanner *LEFData::getLexer()
	{
		return lexer;
	}
}

