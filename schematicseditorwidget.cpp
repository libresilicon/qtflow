#include "schematicseditorwidget.h"

SchematicsEditorWidget::SchematicsEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new SchematicsEditor(this))
{
	setEditWidget(editArea);
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
