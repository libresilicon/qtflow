
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTextEditor/View>

#include "codeeditorwidget.h"

CodeEditorWidget::CodeEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	edit(KTextEditor::Editor::instance()),
	doc(edit->createDocument(this)),
	view(doc->createView(this)),
	fileInfo(QFileInfo())
{
	QAction *button;
	QToolBar *toolbar = new QToolBar(this);

	setCentralWidget(view);
	setType(VerilogCodeEditorWidgetType);
	connect(doc,SIGNAL(textChanged(KTextEditor::Document*)),this,SLOT(onContentChanged()));

	button = new QAction(QPixmap(":/simulator.svg"), "Run simulation", toolbar);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(runSimulation()));
	toolbar->addAction(button);

	button = new QAction(QPixmap(":/component_select_unit.svg"), "Run synthesis", toolbar);
	connect(button, SIGNAL(triggered(bool)), this, SLOT(runSynthesis()));
	toolbar->addAction(button);

	addToolBar(toolbar);
}

void CodeEditorWidget::saveFile()
{
	QString filePath = fileInfo.absoluteFilePath();
	if (filePath == QString())
		return;

	doc->saveAs(QUrl::fromLocalFile(filePath));
	setStatusChanged(false);
	emit(contentSaved());
}

void CodeEditorWidget::loadFile(QString path)
{
	fileInfo = QFileInfo(path);

	QString filePath = fileInfo.absoluteFilePath();
	if (filePath == QString())
		return;

	doc->openUrl(QUrl::fromLocalFile(filePath));
	setStatusChanged(false);
}

void CodeEditorWidget::setProject(Project *p)
{
	project = p;
}

QString CodeEditorWidget::getFilePath()
{
	return fileInfo.absoluteFilePath();
}

void CodeEditorWidget::onContentChanged()
{
	setStatusChanged(true);
	emit(contentChanged());
}

void CodeEditorWidget::runSynthesis()
{
	project->synthesis();
}

void CodeEditorWidget::runSimulation()
{
	project->simulation();
}
