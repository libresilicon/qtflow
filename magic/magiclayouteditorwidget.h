#ifndef MAGICLAYOUTEDITORWIDGET_H
#define MAGICLAYOUTEDITORWIDGET_H

#include <QDockWidget>
#include <QMainWindow>

#include "magiclayouteditor.h"
#include "qlayout/genericlayouteditorwidget.h"
#include "magic3d/magic3d.h"

#include "qlayout/layoutvisibles.h"

class MagicLayoutEditorWidget : public GenericLayoutEditorWidget
{
	Q_OBJECT
public:
	MagicLayoutEditorWidget(QWidget *parent = nullptr);

public slots:
	void show3D();

private:
	Magic3D *view3D;
};

#endif // MAGICLAYOUTEDITORWIDGET_H
