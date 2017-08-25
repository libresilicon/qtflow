#include "editorwidget.h"

EditorWidget::EditorWidget(QWidget *parent) :
	QWidget(parent),
	layout(new QVBoxLayout(this)),
	editArea(new Editor(this))
{
	QPixmap pixmapDocumentSave(":/document-save.svg");
	QAction *saveButton;

	toolbar = new QToolBar(this);
	saveButton = new QAction(pixmapDocumentSave,"S&ave", toolbar);

	toolbar->addAction(saveButton);
	layout->setMenuBar(toolbar);
	layout->addWidget(editArea);

	this->setLayout(layout);

	//connect(editorWidget,SIGNAL(textChanged()),parent,);
}

void EditorWidget::loadFile(QString path)
{
	QFileInfo info(path);
	editArea->loadFile(path);
	if(info.suffix()=="v") {
		editArea->setSyntax(new VerilogHighlight(editArea->document()));
	}
}
