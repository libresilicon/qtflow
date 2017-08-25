#ifndef CODEEDITORWIDGET_H
#define CODEEDITORWIDGET_H

#include "editorwidget.h"
#include "verilog.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>

class CodeEditorWidget : public EditorWidget
{
	Q_OBJECT
public:
	explicit CodeEditorWidget(QWidget *parent = nullptr);
};

#endif // EDITORWIDGET_H
