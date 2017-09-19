#ifndef MAGICLAYOUTEDITORWIDGET_H
#define MAGICLAYOUTEDITORWIDGET_H

#include <QDockWidget>

#include "magiclayouteditor.h"
#include "editorwidget.h"
#include "magic3d/magic3d.h"

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
	void show3D();

private:
	MagicLayoutEditor *editArea;
	Project *project;
	Magic3D *view3D;
};

#endif // MAGICLAYOUTEDITORWIDGET_H
