#ifndef I_EDITOR_H
#define I_EDITOR_H

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

#endif
