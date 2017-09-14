#include "magiclayouteditorwidget.h"

MagicLayoutEditorWidget::MagicLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new MagicLayoutEditor(this))
{
	setEditWidget(editArea);
}

void MagicLayoutEditorWidget::loadFile(QString path)
{
	editArea->loadFile(path);
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
