
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTextEditor/View>

#include "codeeditorwidget.h"

CodeEditorWidget::CodeEditorWidget(QWidget *parent) :
	EditorWidget(parent),
    editArea(KTextEditor::Editor::instance()),
    doc(NULL),
    view(NULL),
    fileInfo(QFileInfo())
{
	setType(VerilogCodeEditorWidgetType);
	connect(editArea,SIGNAL(blockCountChanged(int)),this,SLOT(onContentChanged()));
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
    doc = editArea->createDocument(this);
    view = doc->createView(this);
    setCentralWidget(view);

    fileInfo = QFileInfo(path);
    doc->openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
	setStatusChanged(false);
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
