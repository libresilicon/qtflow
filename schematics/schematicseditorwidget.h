#ifndef SCHEMATICEDITORWIDGET_H
#define SCHEMATICEDITORWIDGET_H

#include "schematicseditor.h"
#include "editorwidget.h"

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

	void setProject(Project *p);

public slots:
	void onContentChanged();
	void saveFile();

private:
	SchematicsEditor *editArea;
	Project *project;
};

#endif // EDITORWIDGET_H
