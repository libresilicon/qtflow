#ifndef QLAYOUTSCENE_H
#define QLAYOUTSCENE_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QRubberBand>
#include <QGraphicsProxyWidget>
#include <QShortcut>

#include "qlayoutrectitem.h"
#include "qlayoutmacroitem.h"

#include "project.h"

#include "lef/lefdata.h"

enum drawing_operations {
	DRAWING_OPERATION_NONE,
	DRAWING_OPERATION_SELECT,
	DRAWING_OPERATION_RECTANGLE,
	DRAWING_OPERATION_POLYGON,
	DRAWING_OPERATION_CUT_OUT,
	DRAWING_OPERATION_DRAG
};

typedef QVector<QGraphicsRectItem*> layer_macro_wires_t;
typedef QVector<QLayoutRectItem*> rects_layer_t;

class QLayoutScene : public QGraphicsScene
{
public:
	QLayoutScene(QObject *parent = Q_NULLPTR);
	QLayoutScene(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
	QLayoutScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);

	void setProject(Project *p);
	void setLEF(lef::LEFData *d);

	void setDrawingOperation(drawing_operations o);
	void setActiveLayer(QString layer);
	void setVisibleLayers(QStringList l);
	void redraw();

	void addWire(QString layer, int x, int y, int w, int h);
	void addRectangle(QString layer, int x, int y, int w, int h);
	void addMacro(QString module_name, QString instance, int x, int y, int w, int h);

	QStringList getLayers();
	QVector<QLayoutRectItem*> getRectangles(QString n);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	void keyPressEvent(QKeyEvent *event);

	void drawBackground(QPainter *painter, const QRectF &rect);
	void resizeEvent(QResizeEvent *event);

signals:
	void contentChanged();
	void contentSaved();

private:
	QPointF snapGrid(QPointF pt);
	int countSelectedRectItems(QVector<QLayoutRectItem*> l);

	Project *project;
	lef::LEFData *lefdata;

	QString activeLayer;
	QLayoutRectItem *recentRectangle;
	QGraphicsRectItem *recentSelectRectangle;
	drawing_operations recentOperation;
	QPointF lastOrig;
	QStringList visibleLayers;
	bool m_dragging;
	int m_gridSize;

	QVector<QLayoutMacroItem*> macros;
	QVector<QGraphicsTextItem*> macro_texts;
	QMap<QString,layer_macro_wires_t> macro_wires;
	QMap<QString,rects_layer_t> layer_rects;
};

#endif // QLAYOUTSCENE_H
