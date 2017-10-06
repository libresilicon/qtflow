#ifndef MAGICLAYOUTEDITOR_H
#define MAGICLAYOUTEDITOR_H

#include "ieditor.h"
#include "magic/magicdata.h"
#include "lef/lefdata.h"
#include "project.h"
#include "layoutvisibles.h"

#include "qlayout/qlayoutscene.h"

#include <QGridLayout>
#include <QAbstractScrollArea>
#include <QFileInfo>
#include <QResource>
#include <QDebug>
#include <QTemporaryDir>
#include <QAction>
#include <QComboBox>
#include <QGraphicsView>

namespace magic {
	class MagicData;
}

namespace lef {
	class LEFData;
}

class MagicLayoutEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	explicit MagicLayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
	void setProject(Project *p);
	void setVisibles(LayoutVisibles *v);
	void setActiveLayerSelection(QComboBox *s);
	void saveFile();

	QString getFilePath();

	void setDrawingOperation(drawing_operations o);

	bool changes();

signals:
	void contentChanged();
	void contentSaved();

public slots:
	void visibles_action(QString s);

	void setActiveLayer(QString);

private:
	void setRecentVisible(QString s);

	/* magic output functions */
	void saveFileWriteHeader(QTextStream &outputStream);
	void saveFileWriteRects(QTextStream &outputStream);
	void saveFileWriteMacros(QTextStream &outputStream);
	/* end of magic output functions */

	void addRectangles();
	void addMacroInstances();

	QString filePath;
	QLayoutScene *editScene;
	magic::MagicData *magicdata;
	lef::LEFData *lefdata;

	QRectF sceneRect;
	Project *project;
	LayoutVisibles *visibles;
	QComboBox *activeLayerSelection;
};

#endif // MAGICLAYOUTEDITOR_H
