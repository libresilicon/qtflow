#include "deflayouteditorwidget.h"

DEFLayoutEditorWidget::DEFLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new DEFLayoutEditor(this))
{
	setEditWidget(editArea);
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
