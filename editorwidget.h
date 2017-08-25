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
	void loadFile(QString);
	QString getFilePath();
	
	bool getStatusChanged();

signals:
	void textChanged();
	void textSaved();
	
public slots:
	void onTextChanged();
	void saveFile();

private:
	Editor *editArea;
	QVBoxLayout *layout;
	QToolBar *toolbar;
	
	bool statusChanged;
};

#endif // EDITORWIDGET_H
