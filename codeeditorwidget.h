#ifndef CODEEDITORWIDGET_H
#define CODEEDITORWIDGET_H

#include "editorwidget.h"

#include "project.h"

#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KTextEditor/View>

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>
#include <QErrorMessage>
#include <QMessageBox>

class CodeEditorWidget : public EditorWidget
{
	Q_OBJECT
public:
	explicit CodeEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	QString getFilePath();
	void setProject(Project *p);

public slots:
	void onContentChanged();
	void runSimulation();
	void runSynthesis();
	void saveFile();

private:
    KTextEditor::Editor *edit;
    KTextEditor::Document *doc;
    KTextEditor::View *view;

    QFileInfo fileInfo;
	Project *project;
};

#endif // EDITORWIDGET_H
