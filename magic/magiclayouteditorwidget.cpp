#include "magiclayouteditorwidget.h"

MagicLayoutEditorWidget::MagicLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	project(NULL),
	editArea(new MagicLayoutEditor(this)),
	view3D(new Magic3D(this))
{
	QAction *button;
	QToolBar *toolbar = new QToolBar(this);

	setType(MagicLayoutEditorWidgetType);

	button = new QAction(QPixmap(":/three_d.svg"), "3D view", toolbar);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(show3D()));
	toolbar->addAction(button);

	/*button = new QAction(QPixmap(":/three_d.svg"), "3D view", toolbar);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(show3D()));
	toolbar->addAction(button);*/

	addToolBar(toolbar);

	setCentralWidget(editArea);

	layoutVisibles = new LayoutVisibles(this);
	layoutVisibles->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, layoutVisibles);
	editArea->setVisibles(layoutVisibles);

	connect(editArea, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));
}

void MagicLayoutEditorWidget::onContentChanged()
{
	setStatusChanged(true);
	emit(contentChanged());
}

void MagicLayoutEditorWidget::addDrawingLayerSelection()
{
	if(!project) return;

	QToolBar *toolbar;

	toolbar = new QToolBar(this);
	activeLayer = new QComboBox(toolbar);

	foreach(QString layern, project->getPlanes()) {
		foreach(QString vname, project->getType(layern)) {
			activeLayer->addItem(project->materialIcon(vname),vname);
		}
	}

	toolbar->addWidget(activeLayer);
	addToolBar(toolbar);
}

void MagicLayoutEditorWidget::addDrawingOperations()
{
	if(!project) return;

	QAction *button;
	QToolBar *toolbar;

	toolbar = new QToolBar(this);

	button = new QAction(QPixmap(":/add_rectangle.svg"), "Add rectangle", toolbar);
	button->setCheckable(true);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperationAddRectangle()));
	drawingOperations[DRAWING_OPERATION_RECTANGLE] = button;
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/add_rectangle.svg"), "Select rectangles", toolbar);
	button->setCheckable(true);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperationSelectRectangles()));
	drawingOperations[DRAWING_OPERATION_SELECT] = button;
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/drag.svg"), "Drag rectangles", toolbar);
	button->setCheckable(true);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperationDragRectangles()));
	drawingOperations[DRAWING_OPERATION_DRAG] = button;
	toolbar->addAction(button);

	addToolBar(toolbar);
}

void MagicLayoutEditorWidget::disableAllDrawingOperationsExcept(drawing_operations o)
{
	foreach(drawing_operations k, drawingOperations.keys()) {
		if(k!=o) drawingOperations[k]->setChecked(false);
	}
}

void MagicLayoutEditorWidget::drawingOperationAddRectangle()
{
	editArea->setDrawingOperation(DRAWING_OPERATION_NONE);
	disableAllDrawingOperationsExcept(DRAWING_OPERATION_RECTANGLE);
	if(drawingOperations.contains(DRAWING_OPERATION_RECTANGLE)) {
		if(drawingOperations[DRAWING_OPERATION_RECTANGLE]->isChecked())
			editArea->setDrawingOperation(DRAWING_OPERATION_RECTANGLE);
	}
}

void MagicLayoutEditorWidget::drawingOperationSelectRectangles()
{
	editArea->setDrawingOperation(DRAWING_OPERATION_NONE);
	disableAllDrawingOperationsExcept(DRAWING_OPERATION_SELECT);
	if(drawingOperations.contains(DRAWING_OPERATION_SELECT)) {
		if(drawingOperations[DRAWING_OPERATION_SELECT]->isChecked())
			editArea->setDrawingOperation(DRAWING_OPERATION_SELECT);
	}
}

void MagicLayoutEditorWidget::drawingOperationDragRectangles()
{
	editArea->setDrawingOperation(DRAWING_OPERATION_NONE);
	disableAllDrawingOperationsExcept(DRAWING_OPERATION_DRAG);
	if(drawingOperations.contains(DRAWING_OPERATION_DRAG)) {
		if(drawingOperations[DRAWING_OPERATION_DRAG]->isChecked())
			editArea->setDrawingOperation(DRAWING_OPERATION_DRAG);
	}
}

void MagicLayoutEditorWidget::loadFile(QString path)
{
	editArea->setProject(project);
	editArea->setVisibles(layoutVisibles);
	editArea->setActiveLayerSelection(activeLayer);

	editArea->loadFile(path);

	view3D->setProject(project);
	view3D->loadFile(path);
}

void MagicLayoutEditorWidget::show3D()
{
	view3D->show();
}

void MagicLayoutEditorWidget::setProject(Project *p)
{
	project = p;
	layoutVisibles->setProject(project);
	editArea->setProject(project);
	addDrawingOperations();
	addDrawingLayerSelection();
}

QString MagicLayoutEditorWidget::getFilePath()
{
	return editArea->getFilePath();
}

void MagicLayoutEditorWidget::saveFile()
{
	editArea->saveFile();
	setStatusChanged(false);
	emit(contentSaved());
}
