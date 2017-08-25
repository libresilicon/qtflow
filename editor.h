#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QPainter>
#include <QTextBlock>
#include <QTextStream>

class IEditor
{
public:
	IEditor() {}
	virtual ~IEditor() {}

	virtual void lineNumberAreaPaintEvent(QPaintEvent*) {}
	virtual int lineNumberAreaWidth() { return 0; }

	virtual void loadFile(QString) {}
	virtual void saveFile(QString) {}
	virtual void saveFile() {}
	virtual QString getFilePath() { return ""; }

	virtual void setSyntax(QSyntaxHighlighter*) {}

	virtual bool changes() { return false; }
};

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
