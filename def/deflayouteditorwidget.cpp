#include "deflayouteditorwidget.h"

DEFLayoutEditorWidget::DEFLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new DEFLayoutEditor(this))
{
	QToolBar *toolbar = new QToolBar(this);
	QAction *button;

	setType(DEFLayoutEditorWidgetType);
	setCentralWidget(editArea);

	button = new QAction(QPixmap(":/zoom_in.svg"), "Zoom in", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(zoomIn()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/zoom_out.svg"), "Zoom out", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(zoomOut()));
	toolbar->addAction(button);

	addToolBar(toolbar);
}

void DEFLayoutEditorWidget::loadFile(QString path)
{
	editArea->loadFile(path);
}

QString DEFLayoutEditorWidget::getFilePath()
{
	return editArea->getFilePath();
}

void DEFLayoutEditorWidget::onContentChanged()
{
}

void DEFLayoutEditorWidget::saveFile()
{
}

void DEFLayoutEditorWidget::setProject(Project *p)
{
	project = p;
	//layoutVisibles->setProject(project);
	editArea->setProject(project);
	//addDrawingOperations();
	//addDrawingLayerSelection();
}
