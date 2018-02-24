#ifndef SCHEMATICEDITORWIDGET_H
#define SCHEMATICEDITORWIDGET_H

#include "schematicseditor.h"
#include "qeditor/editorwidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>

class SchematicsEditorWidget : public EditorWidget
{
	Q_OBJECT

public:
	explicit SchematicsEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	QString getFilePath();

public slots:
	void drawingOperation();

	void onContentChanged();
	void saveFile();

private:
	void addDrawingOperations();

	SchematicsEditor *editArea;

	QMap<schematics_operations,QAction*> drawingOperations;
};

#endif // EDITORWIDGET_H
