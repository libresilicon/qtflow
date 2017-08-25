#include "editorwidget.h"

EditorWidget::EditorWidget(QWidget *parent) :
	QWidget(parent),
	layout(new QVBoxLayout(this)),
	editArea(new Editor(this)),
	statusChanged(false)
{
	QPixmap pixmapDocumentSave(":/document-save.svg");
	QAction *saveButton;

	toolbar = new QToolBar(this);
	saveButton = new QAction(pixmapDocumentSave,"S&ave", toolbar);

	toolbar->addAction(saveButton);
	layout->setMenuBar(toolbar);
	layout->addWidget(editArea);

	this->setLayout(layout);

	//connect(editArea,SIGNAL(textChanged()),this,SLOT(onTextChanged()));
	connect(editArea,SIGNAL(blockCountChanged(int)),this,SLOT(onTextChanged()));
}

void EditorWidget::saveFile()
{
	editArea->saveFile();
	statusChanged = false;
}

void EditorWidget::loadFile(QString path)
{
	QFileInfo info(path);
	editArea->loadFile(path);
	if(info.suffix()=="v") {
		editArea->setSyntax(new VerilogHighlight(editArea->document()));
	}
	statusChanged = false;
}

bool EditorWidget::getStatusChanged()
{
	return statusChanged;
}

QString EditorWidget::getFilePath()
{
	editArea->getFilePath();
}

void EditorWidget::onTextChanged()
{
	statusChanged = true;
	emit(textChanged(editArea->getFilePath()));
}