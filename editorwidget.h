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
	void saveFile();
	QString getFilePath();
	
	bool getStatusChanged();

signals:
	void textChanged(QString);
	
public slots:
	void onTextChanged();

private:
	Editor *editArea;
	QVBoxLayout *layout;
	QToolBar *toolbar;
	
	bool statusChanged;
};

#endif // EDITORWIDGET_H
