#ifndef DEFLAYOUTEDITOR_H
#define DEFLAYOUTEDITOR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGridLayout>
#include <QTemporaryDir>
#include <QAbstractScrollArea>

#include "qlayout/qlayoutscene.h"
#include "qlayout/layoutvisibles.h"
#include "defdata.h"

#include "qlayout/genericlayouteditor.h"

namespace def {
	class DEFData;
}

class DEFLayoutEditor : public GenericLayoutEditor
{
	Q_OBJECT

public:
	explicit DEFLayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
	void saveFile();

private:
	void addMacroInstances();
	void addContactPins();
	void addRectangles();
	void addSignalWires();

	def::DEFData *defdata;
};

#endif // DEFLAYOUTEDITOR_H
