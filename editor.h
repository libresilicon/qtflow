#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QTextBlock>
#include <QTextStream>

#include "ieditor.h"

class Editor : public QPlainTextEdit, public IEditor
{
	Q_OBJECT

public:
	Editor(QWidget *parent = 0);
	~Editor();

	void lineNumberAreaPaintEvent(QPaintEvent*);
	int lineNumberAreaWidth();

	void loadFile(QString);
	void saveFile(QString);
	void saveFile();
	QString getFilePath();

	void setSyntax(QSyntaxHighlighter*);

	bool changes();

protected:
	void resizeEvent(QResizeEvent*) override;

private slots:
	void updateLineNumberAreaWidth(int);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect&, int);

private:
	QWidget *lineNumberArea;
	QSyntaxHighlighter *highlight;
	QString filePath;
};

class LineNumberArea : public QWidget
{
private:
	Editor *codeEditor;

public:
	LineNumberArea(Editor *editor) : QWidget(editor)
	{
		codeEditor = editor;
	}
	~LineNumberArea() {}

	QSize sizeHint() const override
	{
		return QSize(codeEditor->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event) override
	{
		codeEditor->lineNumberAreaPaintEvent(event);
	}
};

#endif // EDITOR_H
