
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
    setCentralWidget(view);
    setType(VerilogCodeEditorWidgetType);
    connect(edit,SIGNAL(blockCountChanged(int)),this,SLOT(onContentChanged()));
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
