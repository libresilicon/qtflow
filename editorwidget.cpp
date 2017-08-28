#include "editorwidget.h"

#include <QShortcut>

EditorWidget::EditorWidget(QWidget *parent) :
	QWidget(parent),
	toolbar(new QToolBar(this)),
	layout(new QGridLayout(this)),
	statusChanged(false)
{
	QPixmap pixmapDocumentSave(":/document-save.svg");
	QAction *saveButton;

	saveButton = new QAction(pixmapDocumentSave,"S&ave", toolbar);
	connect(saveButton,SIGNAL(triggered(bool)),this,SLOT(saveFile()));

	QShortcut *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
	connect(saveShortcut, SIGNAL(activated()),this,SLOT(saveFile()));

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

void EditorWidget::loadFile(QString path) { filePath = path; }
QString EditorWidget::getFilePath() { return filePath; }
void EditorWidget::onContentChanged() {}
void EditorWidget::saveFile() {}
