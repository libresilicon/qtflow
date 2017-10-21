#ifndef SCHEMATICSEDITOR_H
#define SCHEMATICSEDITOR_H

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QLabel>

#include "ieditor.h"

#include "qschematics/qschematicsscene.h"

#include "schematics_reader/schematicsdata.h"

#include "schematicspartselection.h"
#include "schematicslibraryeditor.h"

class SchematicsEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	SchematicsEditor(QWidget *parent = 0);

	void loadFile(QString);
	void saveFile();
	QString getFilePath();

	bool changes();

	void setProject(Project *p);

public slots:
	void zoomIn();
	void zoomOut();
	void showPartSelection();
	void showLibraryEditor();

	void onInsertPart(QString name);

protected:

private:
	void addWires();
	void addParts();

	QString filePath;
	QSchematicsScene *editScene;
	schematics::SchematicsData *schematicsdata;

	Project *project;

	SchematicsPartSelection *partSelection;
	SchematicsLibraryEditor *libraryEditor;
};

#endif // SCHEMATICSEDITOR_H
