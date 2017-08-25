#ifndef SCHEMATICSEDITOR_H
#define SCHEMATICSEDITOR_H

#include "ieditor.h"
#include <QWidget>
#include <QPainter>
#include <QGraphicsWidget>

class SchematicsEditor : public QWidget, public IEditor
{
	Q_OBJECT

public:
	SchematicsEditor(QWidget *parent = 0);
	~SchematicsEditor();

	void loadFile(QString);
	void saveFile();
	QString getFilePath();

	bool changes();

protected:
	void resizeEvent(QResizeEvent*) override;
	void paintEvent(QPaintEvent *event);

private:
	QString filePath;
};

#endif // SCHEMATICSEDITOR_H
