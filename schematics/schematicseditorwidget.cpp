#include "schematicseditorwidget.h"

SchematicsEditorWidget::SchematicsEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new SchematicsEditor(this)),
	project(NULL)
{
	QAction *button;
	QToolBar *toolbar = new QToolBar(this);

	button = new QAction(QPixmap(":/zoom_in.svg"), "Zoom in", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(zoomIn()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/zoom_out.svg"), "Zoom out", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(zoomOut()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/libedit.svg"), "Edit library", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(showLibraryEditor()));
	toolbar->addAction(button);

	addToolBar(toolbar);

	setCentralWidget(editArea);
}

void SchematicsEditorWidget::loadFile(QString path)
{
	editArea->loadFile(path);
}

QString SchematicsEditorWidget::getFilePath()
{
	return editArea->getFilePath();
}

void SchematicsEditorWidget::onContentChanged()
{
}

void SchematicsEditorWidget::saveFile()
{
}

void SchematicsEditorWidget::setProject(Project *p)
{
	project = p;
	editArea->setProject(project);
	addDrawingOperations();
}

void SchematicsEditorWidget::drawingOperation()
{
	QObject* obj = sender();
	QAction *action = qobject_cast<QAction *>(obj);
	schematics_operations o = (schematics_operations)action->data().toInt();
	editArea->setDrawingOperation(SCHEMATICS_OPERATION_NONE);

	foreach(schematics_operations k, drawingOperations.keys()) {
		if(k!=o) drawingOperations[k]->setChecked(false);
	}

	if(drawingOperations.contains(o)) {
		if(drawingOperations[o]->isChecked())
			editArea->setDrawingOperation(o);
	}
}

void SchematicsEditorWidget::addDrawingOperations()
{
	if(!project) return;

	QAction *button;
	QToolBar *toolbar;

	toolbar = new QToolBar(this);

	button = new QAction(QPixmap(":/add_dashed_line.svg"), "Select items", toolbar);
	button->setCheckable(true);
	button->setData(SCHEMATICS_OPERATION_SELECT);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	drawingOperations[SCHEMATICS_OPERATION_SELECT] = button;
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/drag.svg"), "Drag rectangles", toolbar);
	button->setCheckable(true);
	button->setData(SCHEMATICS_OPERATION_DRAG);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperation()));
	drawingOperations[SCHEMATICS_OPERATION_DRAG] = button;
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/add_component.svg"), "Insert part", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(showPartSelection()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/save_part_in_mem.svg"), "Insert parametric part", toolbar);
	connect(button, SIGNAL(triggered(bool)), editArea, SLOT(showParametricPartSelection()));
	toolbar->addAction(button);

	addToolBar(Qt::RightToolBarArea,toolbar);
}
