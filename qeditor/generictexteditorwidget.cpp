#include "generictexteditorwidget.h"

GenericTextEditorWidget::GenericTextEditorWidget(QWidget *parent) :
	EditorWidget(parent),
	edit(KTextEditor::Editor::instance()),
	doc(edit->createDocument(this)),
	view(doc->createView(this)),
	fileInfo(QFileInfo())
{
	setCentralWidget(view);
	setType(VerilogCodeEditorWidgetType);
	connect(doc,SIGNAL(textChanged(KTextEditor::Document*)),this,SLOT(onContentChanged()));
}

void GenericTextEditorWidget::saveFile()
{
	QString filePath = fileInfo.absoluteFilePath();
	if (filePath == QString())
		return;

	doc->saveAs(QUrl::fromLocalFile(filePath));
	setStatusChanged(false);
	emit(contentSaved());
}

void GenericTextEditorWidget::loadFile(QString path)
{
	fileInfo = QFileInfo(path);

	QString filePath = fileInfo.absoluteFilePath();
	if (filePath == QString())
		return;

	doc->openUrl(QUrl::fromLocalFile(filePath));
	setStatusChanged(false);
}

QString GenericTextEditorWidget::getFilePath()
{
	return fileInfo.absoluteFilePath();
}

void GenericTextEditorWidget::onContentChanged()
{
	setStatusChanged(true);
	emit(contentChanged());
}
