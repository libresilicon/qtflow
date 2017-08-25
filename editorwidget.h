#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include "editor.h"
#include "verilog.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>

class EditorWidget : public QWidget
{
	Q_OBJECT

public:
	explicit EditorWidget(QWidget *parent = nullptr);
	virtual void loadFile(QString);
	virtual QString getFilePath();

	bool getStatusChanged();
	void setEditWidget(QWidget *editArea);
	void setStatusChanged(bool);

signals:
	void contentChanged();
	void contentSaved();

public slots:
	virtual void onContentChanged();
	virtual void saveFile();

private:
	QVBoxLayout *layout;
	QToolBar *toolbar;
	
	bool statusChanged;
};

#endif // EDITORWIDGET_H
