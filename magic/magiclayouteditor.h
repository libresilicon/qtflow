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
#include <QAbstractScrollArea>
#include <QFileInfo>
#include <QResource>
#include <QDebug>
#include <QTemporaryDir>
#include <QAction>

namespace magic {
	class MagicData;
}

namespace lef {
	class LEFData;
}

class ModuleAreaInfo
{
public:
	ModuleAreaInfo();
	bool isSelected;
	QRect area;
};

class MagicLayoutEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	explicit MagicLayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
	void setProject(Project *p);
	void saveFile();
	QString getFilePath();

	void redraw();

	bool changes();

public slots:
	void mousePressEvent(QMouseEvent * e);
	void resizeEvent(QResizeEvent *event);
	void scrollContentsBy(int dx, int dy);

protected:
	QColor colorMat(QString material);
	void drawRectangles();
	void drawModuleInfo();

private:
	QString filePath;
	QGraphicsScene *editScene;
	magic::MagicData *magicdata;
	lef::LEFData *lefdata;

	QMap<QString,ModuleAreaInfo> moduleAreas;
	QRectF sceneRect;
	Project *project;
};

#endif // MAGICLAYOUTEDITOR_H
