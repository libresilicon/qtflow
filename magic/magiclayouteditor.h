#ifndef MAGICLAYOUTEDITOR_H
#define MAGICLAYOUTEDITOR_H

#include "ieditor.h"
#include "magic/magicdata.h"
#include "lef/lefdata.h"
#include "project.h"
#include "layoutvisibles.h"
#include "qgraphicswireitem.h"

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

typedef QVector<QGraphicsRectItem*> layer_t;
typedef QVector<QGraphicsWireItem*> wire_layer_t;

class MagicLayoutEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	explicit MagicLayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
	void setProject(Project *p);
	void setVisibles(LayoutVisibles *v);
	void saveFile();
	QString getFilePath();

	bool changes();

public slots:
	void redraw();
	void resizeEvent(QResizeEvent *event);
	void scrollContentsBy(int dx, int dy);

protected:

private:
	void addWires();
	void addModules();

	QString filePath;
	QGraphicsScene *editScene;
	magic::MagicData *magicdata;
	lef::LEFData *lefdata;

	QRectF sceneRect;
	Project *project;
	LayoutVisibles *visibles;

	QMap<QString,layer_t> layers;
	QMap<QString,wire_layer_t> wires;
	QVector<QGraphicsRectItem*> bounding_boxes;
	QVector<QGraphicsTextItem*> instance_labels;
};

#endif // MAGICLAYOUTEDITOR_H
