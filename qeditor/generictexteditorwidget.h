#ifndef GENERICTEXTEDITORWIDGET_H
#define GENERICTEXTEDITORWIDGET_H

#include "editorwidget.h"

#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KTextEditor/View>

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>
#include <QErrorMessage>
#include <QMessageBox>

class GenericTextEditorWidget : public EditorWidget
{
	Q_OBJECT
public:
	explicit GenericTextEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	QString getFilePath();

public slots:
	void onContentChanged();
	void saveFile();

private:
	KTextEditor::Editor *edit;
	KTextEditor::Document *doc;
	KTextEditor::View *view;

	QFileInfo fileInfo;
};

#endif // GENERICTEXTEDITORWIDGET_H
