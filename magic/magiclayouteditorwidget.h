#ifndef MAGICLAYOUTEDITORWIDGET_H
#define MAGICLAYOUTEDITORWIDGET_H

#include <QDockWidget>
#include <QMainWindow>

#include "magiclayouteditor.h"
#include "editorwidget.h"
#include "magic3d/magic3d.h"
#include "project.h"

#include "qlayout/layoutvisibles.h"

class MagicLayoutEditorWidget : public EditorWidget
{
	Q_OBJECT

public:
	MagicLayoutEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	void setProject(Project *p);
	QString getFilePath();

public slots:
	void onContentChanged();
	void saveFile();
	void show3D();

	void drawingOperation();

private:
	void addDrawingOperations();
	void addDrawingLayerSelection();

	QMap<drawing_operations,QAction*> drawingOperations;

	MagicLayoutEditor *editArea;
	Project *project;
	Magic3D *view3D;
	LayoutVisibles *layoutVisibles;
	QComboBox *activeLayer;
};

#endif // MAGICLAYOUTEDITORWIDGET_H
