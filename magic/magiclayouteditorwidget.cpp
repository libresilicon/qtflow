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
	toolbar->addAction(button);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(show3D()));

	addToolBar(toolbar);

	addDrawingOperations();

	setCentralWidget(editArea);

	layoutVisibles = new LayoutVisibles(this);
	layoutVisibles->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, layoutVisibles);
	editArea->setVisibles(layoutVisibles);
}

void MagicLayoutEditorWidget::addDrawingOperations()
{
	QAction *button;
	QToolBar *toolbar;

	toolbar = new QToolBar(this);
	QComboBox *layers = new QComboBox(toolbar);
	foreach(QString n, project->getLayers()) {
		QPixmap pm(100,100);
		pm.fill(project->colorMat(n));
		layers->addItem(QIcon(pm),n);
	}
	toolbar->addWidget(layers);
	addToolBar(toolbar);

	toolbar = new QToolBar(this);

	button = new QAction(QPixmap(":/add_rectangle.svg"), "Add rectangle", toolbar);
	button->setCheckable(true);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(drawingOperationAddRectangle()));
	drawingOperations[DRAWING_OPERATION_RECTANGLE] = button;
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

void MagicLayoutEditorWidget::loadFile(QString path)
{
	editArea->setProject(project);
	editArea->setVisibles(layoutVisibles);

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
}

QString MagicLayoutEditorWidget::getFilePath()
{
	return editArea->getFilePath();
}

void MagicLayoutEditorWidget::onContentChanged()
{
}

void MagicLayoutEditorWidget::saveFile()
{
}
