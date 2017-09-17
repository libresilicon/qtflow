#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QFileInfo>
#include <QShortcut>
#include <QVBoxLayout>

#include "ieditor.h"

enum WidgetType {
	BareEditorWidgetType,
	SchematicsEditorWidgetType,
	MagicLayoutEditorWidgetType,
	DEFLayoutEditorWidgetType,
	VerilogCodeEditorWidgetType
};

class EditorWidget : public QWidget
{
	Q_OBJECT

public:
	explicit EditorWidget(QWidget *parent = nullptr);
	virtual void loadFile(QString);
	virtual QString getFilePath();

	virtual WidgetType getType();
	void setType(WidgetType);

	bool getStatusChanged();
	void setEditWidget(QWidget *editArea);
	void setStatusChanged(bool);
	
	virtual void closeFile();

signals:
	void contentChanged();
	void contentSaved();

public slots:
	virtual void onContentChanged();
	virtual void saveFile();

private:
	QGridLayout *layout;
	QToolBar *toolbar;
	
	bool statusChanged;

	QString filePath;

	WidgetType widgetType;
};

#endif // EDITORWIDGET_H
