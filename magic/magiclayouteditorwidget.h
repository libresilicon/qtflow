#ifndef MAGICLAYOUTEDITORWIDGET_H
#define MAGICLAYOUTEDITORWIDGET_H

#include <QDockWidget>

#include "magiclayouteditor.h"
#include "editorwidget.h"

class MagicLayoutEditorWidget : public EditorWidget
{
	Q_OBJECT

public:
	MagicLayoutEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	void setProject(Project *p);
	QString getFilePath();

public slots:
	void onContentChanged();
	void saveFile();

private:
	MagicLayoutEditor *editArea;
	Project *project;
};

#endif // MAGICLAYOUTEDITORWIDGET_H
