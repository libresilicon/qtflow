#ifndef SCHEMATICSEDITOR_H
#define SCHEMATICSEDITOR_H

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QLabel>

#include "ieditor.h"

#include "qschematics/qschematicsscene.h"

#include "schematics/schematicsdata.h"

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
	QSchematicsScene *editScene;
	schematics::SchematicsData *schematicsdata;
};

#endif // SCHEMATICSEDITOR_H
