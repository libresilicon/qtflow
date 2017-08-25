#ifndef SCHEMATICEDITORWIDGET_H
#define SCHEMATICEDITORWIDGET_H

#include "editorwidget.h"
#include "verilog.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>

class SchematicsEditorWidget : public EditorWidget
{
	Q_OBJECT
public:
	explicit SchematicsEditorWidget(QWidget *parent = nullptr);
};

#endif // EDITORWIDGET_H
