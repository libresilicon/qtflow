#ifndef DEFLAYOUTEDITORWIDGET_H
#define DEFLAYOUTEDITORWIDGET_H

#include "deflayouteditor.h"
#include "qeditor/editorwidget.h"
#include "qlayout/layoutvisibles.h"

class DEFLayoutEditorWidget : public EditorWidget
{
	Q_OBJECT
public:
	DEFLayoutEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	void loadLEF(QString);
	void loadGDS2(QString);
	QString getFilePath();

public slots:
	void onContentChanged();
	void saveFile();

private:
	DEFLayoutEditor *editArea;
	LayoutVisibles *layoutVisibles;
};

#endif // DEFLAYOUTEDITORWIDGET_H
