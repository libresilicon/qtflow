#ifndef LAYOUTEDITORWIDGET_H
#define LAYOUTEDITORWIDGET_H

#include "magiclayouteditor.h"
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
	MagicLayoutEditor *editArea;
};

#endif // LAYOUTEDITORWIDGET_H
