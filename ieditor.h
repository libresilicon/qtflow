#ifndef I_EDITOR_H
#define I_EDITOR_H

#include <QPainter>
#include <QPaintEvent>

class IEditor
{
public:
        IEditor() {}
	virtual ~IEditor() {}

	virtual void loadFile(QString) {}
	virtual void saveFile(QString) {}
	virtual void saveFile() {}
	virtual QString getFilePath() { return ""; }

	virtual bool changes() { return false; }
};

#endif
