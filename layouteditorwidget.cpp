#include "layouteditorwidget.h"

LayoutEditorWidget::LayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new LayoutEditor(this))
{
	setEditWidget(editArea);
}

void LayoutEditorWidget::loadFile(QString path)
{
	editArea->loadFile(path);
}

QString LayoutEditorWidget::getFilePath()
{
	return editArea->getFilePath();
}

void LayoutEditorWidget::onContentChanged()
{
}

void LayoutEditorWidget::saveFile()
{
}
