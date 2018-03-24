#include "magiclayouteditorwidget.h"

MagicLayoutEditorWidget::MagicLayoutEditorWidget(QWidget *parent) :
	GenericLayoutEditorWidget(parent),
	view3D(new Magic3D(this))
{
	QAction *button;
	setType(MagicLayoutEditorWidgetType);

	// setting up all the tool bar content
	setUpCentralWidget(new MagicLayoutEditor(this));

	button = addToolbarAction(QPixmap(":/icon_3d.svg"), "3D view");
	connect(button, SIGNAL(triggered(bool)), this, SLOT(show3D()));
}

void MagicLayoutEditorWidget::show3D()
{
	view3D->show();
}
