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
#include "drcsettings.h"

#include "project.h"

enum drawing_operations {
	DRAWING_OPERATION_NONE,
	DRAWING_OPERATION_SELECT,
	DRAWING_OPERATION_SELECT_LAYER,
	DRAWING_OPERATION_RECTANGLE,
	DRAWING_OPERATION_POLYGON,
	DRAWING_OPERATION_CUT_OUT,
	DRAWING_OPERATION_DRAG
};

typedef QVector<QGraphicsPolygonItem*> layer_gds_wires_t;
typedef QVector<QGraphicsRectItem*> layer_macro_wires_t;
typedef QVector<QLayoutRectItem*> rects_layer_t;

class QLayoutScene : public QGraphicsScene
{
	Q_OBJECT

public:
	QLayoutScene(QObject *parent = Q_NULLPTR);
	QLayoutScene(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
	QLayoutScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);
	void basicInit();

	void setProject(Project *p);
	void showDRC();

	void setDrawingOperation(drawing_operations o);
	void setActiveLayer(QString layer);
	void redraw();

	void addRectangle(QString layer, int x, int y, int w, int h);
	void addMacro(QString module_name, QString instance_name, int x, int y, int w, int h);
	void addMacro(QString macro_name, QString instance_name, int x, int y);

	QStringList getLayers();
	QVector<QLayoutRectItem*> getRectangles(QString n);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	void keyPressEvent(QKeyEvent *event);

	//void drawBackground(QPainter *painter, const QRectF &rect);

public slots:
	void onVisibleLayersChanged(QStringList l);
	void runDRC();

signals:
	void contentChanged();
	void contentSaved();

	void registerLayer(QString);

private:
	void runDRC(QGraphicsRectItem *m);
	void runDRC(QLayoutRectItem *w);
	void runDRC(QGraphicsPolygonItem *p);

	//QPointF snapGrid(QPointF pt);
	int countSelectedRectItems(QVector<QLayoutRectItem*> l);

	Project *project;

	QString activeLayer;
	QLayoutRectItem *recentRectangle;
	QGraphicsRectItem *recentSelectRectangle;
	drawing_operations recentOperation;
	QPointF lastOrig;
	QStringList m_visibleLayers;
	bool m_dragging;

	QVector<QLayoutMacroItem*> macros;
	QVector<QGraphicsTextItem*> macro_texts;
	QMap<QString,layer_macro_wires_t> macro_wires;
	QMap<QString,rects_layer_t> layer_rects;
	QMap<QString,layer_gds_wires_t> layer_gds;
	QVector<QGraphicsLineItem*> gridLines;

	DRCSettings *drcDialog;
};

#endif // QLAYOUTSCENE_H
