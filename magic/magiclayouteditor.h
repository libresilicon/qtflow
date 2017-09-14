#ifndef MAGICLAYOUTEDITOR_H
#define MAGICLAYOUTEDITOR_H

#include "ieditor.h"

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
	void saveFile();
	QString getFilePath();

	void redraw();

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

	QMap<QString,ModuleAreaInfo> moduleAreas;
};

#endif // MAGICLAYOUTEDITOR_H
