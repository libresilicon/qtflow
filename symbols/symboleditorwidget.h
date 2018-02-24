#ifndef SYMBOLEDITORWIDGET_H
#define SYMBOLEDITORWIDGET_H

#include "qeditor/editorwidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>

class SymbolEditorWidget : public EditorWidget
{
	Q_OBJECT
public:
	explicit SymbolEditorWidget(QWidget *parent = nullptr);

	void loadFile(QString);
	QString getFilePath();
};

#endif // SYMBOLEDITORWIDGET_H
