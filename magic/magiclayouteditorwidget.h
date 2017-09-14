#ifndef MAGICLAYOUTEDITORWIDGET_H
#define MAGICLAYOUTEDITORWIDGET_H

#include "magiclayouteditor.h"
#include "editorwidget.h"

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

private:
	MagicLayoutEditor *editArea;
};

#endif // MAGICLAYOUTEDITORWIDGET_H
