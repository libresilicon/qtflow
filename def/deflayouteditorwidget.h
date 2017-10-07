#ifndef DEFLAYOUTEDITORWIDGET_H
#define DEFLAYOUTEDITORWIDGET_H

#include "deflayouteditor.h"
#include "editorwidget.h"

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
};

#endif // DEFLAYOUTEDITORWIDGET_H
