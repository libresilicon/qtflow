#ifndef MAGICLAYOUTEDITOR_H
#define MAGICLAYOUTEDITOR_H

#include "ieditor.h"
#include "magicdata.h"
#include "lef/lefdata.h"
#include "project.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGridLayout>

namespace magic {
	class MagicData;
}

namespace lef {
	class LEFData;
}

class MagicLayoutEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	explicit MagicLayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
	void setProject(Project *p);
	void saveFile();
	QString getFilePath();

	bool changes();

public slots:
	void mousePressEvent(QMouseEvent * e);

protected:
	QColor colorMat(QString material);
	void drawRectangles();
	void drawModuleInfo();

private:
	QString filePath;
	QGraphicsScene *editScene;
	magic::MagicData *magicdata;
	lef::LEFData *lefdata;

	Project *project;
};

#endif // MAGICLAYOUTEDITOR_H
