#ifndef MAGICLAYOUTEDITORWIDGET_H
#define MAGICLAYOUTEDITORWIDGET_H

#include <QDockWidget>
#include <QMainWindow>

#include "magiclayouteditor.h"
#include "qeditor/editorwidget.h"
#include "magic3d/magic3d.h"

#include "qlayout/layoutvisibles.h"

class MagicLayoutEditorWidget : public EditorWidget
{
	Q_OBJECT

public:
	MagicLayoutEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
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
	Magic3D *view3D;
	LayoutVisibles *layoutVisibles;
	QComboBox *activeLayer;
};

#endif // MAGICLAYOUTEDITORWIDGET_H
