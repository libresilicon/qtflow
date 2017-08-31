#ifndef LAYOUTEDITOR_H
#define LAYOUTEDITOR_H

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

class LayoutEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	explicit LayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
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
};

#endif // LAYOUTEDITOR_H
