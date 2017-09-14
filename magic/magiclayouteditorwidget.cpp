#include "magiclayouteditorwidget.h"

MagicLayoutEditorWidget::MagicLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	project(NULL),
	editArea(new MagicLayoutEditor(this))
{
	setEditWidget(editArea);
}

void MagicLayoutEditorWidget::loadFile(QString path)
{
	editArea->setProject(project);
	editArea->loadFile(path);
}

void MagicLayoutEditorWidget::setProject(Project *p)
{
	project = p;
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
