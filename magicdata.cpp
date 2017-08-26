#include "magicdata.h"

namespace magic {
	MagicData::MagicData():
		lexer(new MagicScanner())
	{

	}

	void MagicData::clearParsedElements()
	{
		parsedElements.clear();
	}

	void MagicData::setTitle(QString s)
	{
		title = s;
	}

	class MagicScanner *MagicData::getLexer()
	{
		return lexer;
	}

	void MagicData::addRectangle(int x, int y, int w, int h, QString m)
	{
		// TODO: Add string for identification!!
		QRect objR(x,y,w,h);
		parsedElements.append(objR);
	}
}
