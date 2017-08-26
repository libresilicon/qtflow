#ifndef LAYOUTEDITORWIDGET_H
#define LAYOUTEDITORWIDGET_H

#include "layouteditor.h"
#include "editorwidget.h"

class LayoutEditorWidget : public EditorWidget
{
	Q_OBJECT

public:
	LayoutEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	QString getFilePath();

public slots:
	void onContentChanged();
	void saveFile();

private:
	LayoutEditor *editArea;
	QWidget *grid;
};

#endif // LAYOUTEDITORWIDGET_H
