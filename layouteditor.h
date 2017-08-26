#ifndef LAYOUTEDITOR_H
#define LAYOUTEDITOR_H

#include "ieditor.h"
#include "magicdata.h"

#include <QWidget>
#include <QGraphicsScene>

class LayoutEditor : public QWidget, public IEditor
{
	Q_OBJECT

public:
	explicit LayoutEditor(QWidget *parent = nullptr);
	~LayoutEditor();

	void loadFile(QString);
	void saveFile();
	QString getFilePath();

	bool changes();

protected:
	void resizeEvent(QResizeEvent*);
	void paintEvent(QPaintEvent *event);

private:
	QString filePath;
	magic::MagicData *magicdata;

};

#endif // LAYOUTEDITOR_H
