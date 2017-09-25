#include "magiclayouteditorwidget.h"

MagicLayoutEditorWidget::MagicLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	project(NULL),
	editArea(new MagicLayoutEditor(this)),
	view3D(new Magic3D(this))
{
	QAction *view3DButton;
	QPixmap pixmap3D(":/three_d.svg");

	setType(MagicLayoutEditorWidgetType);

	view3DButton = new QAction(pixmap3D,"3D view", getToolBar());
	getToolBar()->addAction(view3DButton);
	connect(view3DButton, SIGNAL(triggered(bool)), this, SLOT(show3D()));

	setEditWidget(editArea);
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
