#ifndef DEFLAYOUTEDITOR_H
#define DEFLAYOUTEDITOR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGridLayout>
#include <QTemporaryDir>
#include <QAbstractScrollArea>
//#include <QGLWidget>
//#include <QGLFormat>

#include "qlayout/qlayoutscene.h"
#include "qlayout/layoutvisibles.h"
#include "defdata.h"

namespace def {
	class DEFData;
}

class DEFLayoutEditor : public QGraphicsView
{
	Q_OBJECT

public:
	explicit DEFLayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
	void saveFile();
	QString getFilePath();
	void setVisibles(LayoutVisibles *v);

	bool changes();

public slots:
	void zoomIn();
	void zoomOut();
	void showCellManager();

signals:
	void contentChanged();

private:
	void addMacroInstances();
	void addContactPins();
	void addRectangles();
	void addSignalWires();

	QString filePath;
	QLayoutScene *editScene;
	def::DEFData *defdata;
	LayoutVisibles *visibles;
};

#endif // DEFLAYOUTEDITOR_H
