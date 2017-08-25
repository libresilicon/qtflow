#include "codeeditorwidget.h"

CodeEditorWidget::CodeEditorWidget(QWidget *parent) :
	QWidget(parent),
	layout(new QVBoxLayout(this)),
	editArea(new Editor(this)),
	statusChanged(false)
{
	QPixmap pixmapDocumentSave(":/document-save.svg");
	QAction *saveButton;

	toolbar = new QToolBar(this);

	saveButton = new QAction(pixmapDocumentSave,"S&ave", toolbar);
	connect(saveButton,SIGNAL(triggered(bool)),this,SLOT(saveFile()));

	toolbar->addAction(saveButton);
	layout->setMenuBar(toolbar);
	layout->addWidget(editArea);

	this->setLayout(layout);

	connect(editArea,SIGNAL(blockCountChanged(int)),this,SLOT(onTextChanged()));
}

void CodeEditorWidget::saveFile()
{
	editArea->saveFile();
	statusChanged = false;
	emit(textSaved());
}

void CodeEditorWidget::loadFile(QString path)
{
	QFileInfo info(path);
	editArea->loadFile(path);
	if(info.suffix()=="v") {
		editArea->setSyntax(new VerilogHighlight(editArea->document()));
	}
	statusChanged = false;
}

bool CodeEditorWidget::getStatusChanged()
{
	return statusChanged;
}

QString CodeEditorWidget::getFilePath()
{
	return editArea->getFilePath();
}

void CodeEditorWidget::onTextChanged()
{
	statusChanged = true;
	emit(textChanged());
}
