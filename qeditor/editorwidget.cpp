#include "editorwidget.h"

EditorWidget::EditorWidget(QWidget *parent) :
	QMainWindow(parent),
	statusChanged(false),
	widgetType(BareEditorWidgetType)
{
	m_toolbar = new QToolBar(this);

	QPixmap pixmap(":/document-save.svg");
	QAction *button;

	button = new QAction(pixmap,"S&ave", m_toolbar);
	connect(button,SIGNAL(triggered(bool)),this,SLOT(saveFile()));

	QShortcut *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
	connect(saveShortcut, SIGNAL(activated()),this,SLOT(saveFile()));

	m_toolbar->addAction(button);

	addToolBar(m_toolbar);
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
void EditorWidget::closeFile() {}

WidgetType EditorWidget::getType() { return widgetType; }
void EditorWidget::setType(WidgetType t) { widgetType = t; }
