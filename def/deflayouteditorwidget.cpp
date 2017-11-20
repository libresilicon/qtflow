#include "deflayouteditorwidget.h"

DEFLayoutEditorWidget::DEFLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new DEFLayoutEditor(this)),
	project(NULL)
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

	button = new QAction(QPixmap(":/array_module.svg"), "Cell manager", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(showCellManager()));
	toolbar->addAction(button);

	addToolBar(toolbar);

	layoutVisibles = new LayoutVisibles(this);
	layoutVisibles->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, layoutVisibles);
	editArea->setVisibles(layoutVisibles);

	connect(editArea, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));
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
	layoutVisibles->setProject(project);
	editArea->setProject(project);
	//addDrawingOperations();
	//addDrawingLayerSelection();
}
