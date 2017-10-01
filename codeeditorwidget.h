#ifndef CODEEDITORWIDGET_H
#define CODEEDITORWIDGET_H

#include "editorwidget.h"

#include "editor.h"

#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KTextEditor/View>

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>

class CodeEditorWidget : public EditorWidget
{
	Q_OBJECT
public:
	explicit CodeEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	QString getFilePath();

public slots:
	void onContentChanged();
	void saveFile();

private:
    KTextEditor::Editor *editArea;
    KTextEditor::Document *doc;
    KTextEditor::View *view;

    QFileInfo fileInfo;
};

#endif // EDITORWIDGET_H
