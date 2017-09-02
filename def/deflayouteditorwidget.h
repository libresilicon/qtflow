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

public slots:
	void onContentChanged();
	void saveFile();

private:
	DEFLayoutEditor *editArea;
};

#endif // DEFLAYOUTEDITORWIDGET_H
