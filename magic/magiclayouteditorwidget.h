#ifndef MAGICLAYOUTEDITORWIDGET_H
#define MAGICLAYOUTEDITORWIDGET_H

#include <QDockWidget>
#include <QMainWindow>

#include "magiclayouteditor.h"
#include "editorwidget.h"
#include "magic3d/magic3d.h"
#include "layoutvisibles.h"
#include "project.h"

class MagicLayoutEditorWidget : public EditorWidget
{
	Q_OBJECT

public:
	MagicLayoutEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	void setProject(Project *p);
	void setVisibles(LayoutVisibles *v);
	QString getFilePath();

public slots:
	void onContentChanged();
	void saveFile();
	void show3D();

private:
	MagicLayoutEditor *editArea;
	Project *project;
	LayoutVisibles *visibles;
	Magic3D *view3D;
};

#endif // MAGICLAYOUTEDITORWIDGET_H
