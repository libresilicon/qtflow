#include "deflayouteditorwidget.h"

DEFLayoutEditorWidget::DEFLayoutEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new DEFLayoutEditor(this))
{
	setType(DEFLayoutEditorWidgetType);
	setCentralWidget(editArea);
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

void DEFLayoutEditorWidget::setProject(Project *p)
{
	project = p;
	//layoutVisibles->setProject(project);
	editArea->setProject(project);
	//addDrawingOperations();
	//addDrawingLayerSelection();
}
