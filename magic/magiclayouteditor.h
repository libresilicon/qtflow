#ifndef MAGICLAYOUTEDITOR_H
#define MAGICLAYOUTEDITOR_H

#include "magic/magicdata.h"

#include "qlayout/genericlayouteditor.h"

namespace magic {
	class MagicData;
}

class MagicLayoutEditor : public GenericLayoutEditor
{
	Q_OBJECT
public:
	explicit MagicLayoutEditor(QWidget *parent = nullptr);
	void loadFile(QString);
	void saveFile();

private:
	/* magic output functions */
	void saveFileWriteHeader(QTextStream &outputStream);
	void saveFileWriteRects(QTextStream &outputStream);
	void saveFileWriteMacros(QTextStream &outputStream);
	/* end of magic output functions */

	void addRectangles();
	void addMacroInstances();

	magic::MagicData *magicdata;
};

#endif // MAGICLAYOUTEDITOR_H
