#include "codeeditorwidget.h"

CodeEditorWidget::CodeEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	editArea(new Editor(this))
{
	setEditWidget(editArea);
	connect(editArea,SIGNAL(blockCountChanged(int)),this,SLOT(onContentChanged()));
}

void CodeEditorWidget::saveFile()
{
	editArea->saveFile();
	setStatusChanged(false);
	emit(contentSaved());
}

void CodeEditorWidget::loadFile(QString path)
{
	QFileInfo info(path);
	editArea->loadFile(path);
	if(info.suffix()=="v") {
		editArea->setSyntax(new VerilogHighlight(editArea->document()));
	}
	setStatusChanged(false);
}

QString CodeEditorWidget::getFilePath()
{
	return editArea->getFilePath();
}

void CodeEditorWidget::onContentChanged()
{
	setStatusChanged(true);
	emit(contentChanged());
}
