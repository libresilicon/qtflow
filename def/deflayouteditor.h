#ifndef DEFLAYOUTEDITOR_H
#define DEFLAYOUTEDITOR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGridLayout>
#include <QTemporaryDir>
#include <QAbstractScrollArea>
#include <QGLWidget>
#include <QGLFormat>

#include "qlayout/qlayoutscene.h"
#include "ieditor.h"
#include "defdata.h"

namespace def {
	class DEFData;
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
	void addContactPins();
	void addRectangles();

	QString filePath;
	QLayoutScene *editScene;
	def::DEFData *defdata;
	Project *project;
};

#endif // DEFLAYOUTEDITOR_H
