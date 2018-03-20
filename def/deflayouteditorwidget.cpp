#include "deflayouteditorwidget.h"

DEFLayoutEditorWidget::DEFLayoutEditorWidget(QWidget *parent) :
	GenericLayoutEditorWidget(parent)
{
	QAction *button;
	setType(DEFLayoutEditorWidgetType);

	// setting up all the tool bar content
	setUpcentralWidget(new DEFLayoutEditor(this));

	//button = addToolbarAction(QPixmap(":/icon_3d.svg"), "3D view");
	//connect(button, SIGNAL(triggered(bool)), this, SLOT(show3D()));
}
