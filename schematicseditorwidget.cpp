#include "schematicseditorwidget.h"

SchematicsEditorWidget::SchematicsEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new SchematicsEditor(this))
{
	setEditWidget(editArea);
}

void SchematicsEditorWidget::loadFile(QString)
{
}

QString SchematicsEditorWidget::getFilePath()
{
}

void SchematicsEditorWidget::onContentChanged()
{
}

void SchematicsEditorWidget::saveFile()
{
}
