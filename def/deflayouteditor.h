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
#include "ieditor.h"
#include "defdata.h"
#include "lef/lefdata.h"

namespace def {
	class DEFData;
}

namespace lef {
	class LEFData;
}

class DEFLayoutEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	explicit DEFLayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
	void saveFile();
	QString getFilePath();
	void setProject(Project *p);

	bool changes();

public slots:
	void zoomIn();
	void zoomOut();

private:
	void addMacroInstances();
	void addRectangles();

	QString filePath;
	QLayoutScene *editScene;
	def::DEFData *defdata;
	lef::LEFData *lefdata;
	Project *project;
};

#endif // DEFLAYOUTEDITOR_H
