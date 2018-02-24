#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QAction>
#include <QWidget>
#include <QToolBar>
#include <QFileInfo>
#include <QShortcut>
#include <QVBoxLayout>
#include <QMainWindow>

enum WidgetType {
	BareEditorWidgetType,
	SchematicsEditorWidgetType,
	MagicLayoutEditorWidgetType,
	DEFLayoutEditorWidgetType,
	VerilogCodeEditorWidgetType
};

class EditorWidget : public QMainWindow
{
	Q_OBJECT
public:
	explicit EditorWidget(QWidget *parent = nullptr);
	virtual void loadFile(QString);
	virtual QString getFilePath();

	virtual WidgetType getType();
	void setType(WidgetType);

	bool getStatusChanged();
	void setStatusChanged(bool);
	
	virtual void closeFile();

signals:
	void contentChanged();
	void contentSaved();

public slots:
	virtual void onContentChanged();
	virtual void saveFile();

private:
	bool statusChanged;
	QString filePath;
	WidgetType widgetType;
};

#endif // EDITORWIDGET_H
