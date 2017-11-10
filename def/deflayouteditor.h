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
#include "qlayout/layoutvisibles.h"

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
	void setVisibles(LayoutVisibles *v);

	bool changes();

public slots:
	void zoomIn();
	void zoomOut();

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
	Project *project;
	qreal m_scale;
	LayoutVisibles *visibles;
};

#endif // DEFLAYOUTEDITOR_H
