#include "editorwidget.h"

EditorWidget::EditorWidget(QWidget *parent) :
	QWidget(parent),
	layout(new QVBoxLayout(this)),
	statusChanged(false)
{
	QPixmap pixmapDocumentSave(":/document-save.svg");
	QAction *saveButton;

	toolbar = new QToolBar(this);

	saveButton = new QAction(pixmapDocumentSave,"S&ave", toolbar);
	connect(saveButton,SIGNAL(triggered(bool)),this,SLOT(saveFile()));

	toolbar->addAction(saveButton);
	layout->setMenuBar(toolbar);

	this->setLayout(layout);
}

void EditorWidget::setEditWidget(QWidget *editArea)
{
	layout->addWidget(editArea);
}

void EditorWidget::setStatusChanged(bool t)
{
	statusChanged = t;
}

bool EditorWidget::getStatusChanged()
{
	return statusChanged;
}

void EditorWidget::loadFile(QString path) {}
QString EditorWidget::getFilePath() {}
void EditorWidget::onContentChanged() {}
void EditorWidget::saveFile() {}
