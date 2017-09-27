#include "magiclayouteditorwidget.h"

MagicLayoutEditorWidget::MagicLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	project(NULL),
	editArea(new MagicLayoutEditor(this)),
	view3D(new Magic3D(this))
{
	QAction *button;
	QPixmap pixmap(":/three_d.svg");
	QToolBar *toolbar = new QToolBar(this);

	setType(MagicLayoutEditorWidgetType);

	button = new QAction(pixmap, "3D view", this->menuBar());
	toolbar->addAction(button);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(show3D()));

	addToolBar(toolbar);
	setCentralWidget(editArea);
}

void MagicLayoutEditorWidget::loadFile(QString path)
{
	editArea->setProject(project);
	editArea->setVisibles(visibles);

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
}

void MagicLayoutEditorWidget::setVisibles(LayoutVisibles *v)
{
	visibles = v;
	editArea->setVisibles(visibles);
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
