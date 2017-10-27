#ifndef SYMBOLEDITORWIDGET_H
#define SYMBOLEDITORWIDGET_H

#include "editorwidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>

#include "project.h"

class SymbolEditorWidget : public EditorWidget
{
	Q_OBJECT
public:
	explicit SymbolEditorWidget(QWidget *parent = nullptr);

	void loadFile(QString);
	QString getFilePath();

	void setProject(Project *p);
};

#endif // SYMBOLEDITORWIDGET_H
