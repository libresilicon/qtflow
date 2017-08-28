#ifndef SCHEMATICSEDITOR_H
#define SCHEMATICSEDITOR_H

#include "ieditor.h"
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>

class SchematicsEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	SchematicsEditor(QWidget *parent = 0);

	void loadFile(QString);
	void saveFile();
	QString getFilePath();

	bool changes();

protected:

private:
	QString filePath;
	QGraphicsScene *editScene;
};

#endif // SCHEMATICSEDITOR_H
