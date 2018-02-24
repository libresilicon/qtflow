#ifndef SCHEMATICSEDITOR_H
#define SCHEMATICSEDITOR_H

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QLabel>

#include "qschematics/qschematicsscene.h"

#include "schematics_reader/schematicsdata.h"

#include "schematicspartselection.h"
#include "schematicslibraryeditor.h"
#include "schematicsparametricpartdialog.h"

class SchematicsEditor : public QGraphicsView
{
	Q_OBJECT

public:
	SchematicsEditor(QWidget *parent = 0);

	void loadFile(QString);
	void saveFile();
	QString getFilePath();

	bool changes();

	void setDrawingOperation(schematics_operations o);

public slots:
	void zoomIn();
	void zoomOut();
	void showPartSelection();
	void showParametricPartSelection();
	void showLibraryEditor();

	void onInsertPart(QString name);

protected:

private:
	void addWires();
	void addParts();

	QString filePath;
	QSchematicsScene *editScene;
	schematics::SchematicsData *schematicsdata;

	SchematicsPartSelection *partSelection;
	SchematicsLibraryEditor *libraryEditor;
	SchematicsParametricPartDialog *parametricPartSelection;
};

#endif // SCHEMATICSEDITOR_H
