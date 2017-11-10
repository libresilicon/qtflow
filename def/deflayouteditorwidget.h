#ifndef DEFLAYOUTEDITORWIDGET_H
#define DEFLAYOUTEDITORWIDGET_H

#include "deflayouteditor.h"
#include "editorwidget.h"

#include "qlayout/layoutvisibles.h"

class DEFLayoutEditorWidget : public EditorWidget
{
	Q_OBJECT

public:
	DEFLayoutEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	QString getFilePath();
	void setProject(Project *p);

public slots:
	void onContentChanged();
	void saveFile();

private:
	DEFLayoutEditor *editArea;
	Project *project;
	LayoutVisibles *layoutVisibles;
};

#endif // DEFLAYOUTEDITORWIDGET_H
