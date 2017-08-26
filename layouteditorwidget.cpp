#include "layouteditorwidget.h"

LayoutEditorWidget::LayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent)
{
	QWidget *grid = new QWidget(this);
	editArea = new LayoutEditor(grid);
	setEditWidget(grid);
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
