#ifndef QLAYOUTSCENE_H
#define QLAYOUTSCENE_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGLWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QRubberBand>
#include <QGraphicsProxyWidget>
#include <QShortcut>

#include "qlayoutdistancemeasure.h"
#include "qlayoutrectitem.h"
#include "qlayoutmacroitem.h"
#include "qlayoutviaitem.h"

#include "qlayoutcellmanager.h"
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

typedef QVector<QLayoutRectItem*> rects_layer_t;

class QLayoutScene : public QGraphicsScene
{
	Q_OBJECT

public:
	QLayoutScene(QObject *parent = Q_NULLPTR);
	QLayoutScene(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
	QLayoutScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);
	void basicInit();
	void setDistanceUnit(qreal u);

	void setProject(Project *p);
	void showDRC();
	void showCellManager();

	void setDrawingOperation(drawing_operations o);
	void setActiveLayer(QString layer);
	void redraw();

	void addVia(QString netname, QString vianame, QPointF p);
	void addWire(QString netname, QString layer, QPointF p1, QPointF p2);
	void addRectangle(QString layer, qreal x, qreal y, qreal w, qreal h);
	void addPad(QString name, QString net, QString layer, qreal x, qreal y, qreal w, qreal h);
	void addMacro(QString module_name, QString instance_name, qreal x, qreal y, qreal w, qreal h, QString orient);
	void addMacro(QString macro_name, QString instance_name, qreal x, qreal y, QString orient);

	QStringList getLayers();
	QVector<QLayoutRectItem*> getRectangles(QString n);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	void keyPressEvent(QKeyEvent *event);

	//void drawBackground(QPainter *painter, const QRectF &rect);

public slots:
	void setGDS(QString name, bool load);
	void setLEF(QString name, bool load);
	void onVisibleLayersChanged(QStringList l);
	void runDRC();

signals:
	void contentChanged();
	void contentSaved();

	void registerLayer(QString);

private:
	void refreshMacroTable();
	void refreshViaTable();
	void runDRC(QString n, QRectF rect);

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
	qreal m_baseUnit;

	QVector<QLayoutMacroItem*> macros;
	QVector<QGraphicsTextItem*> macro_texts;
	//QMap<QString,layer_macro_wires_t> macro_wires;
	QMap<QString,rects_layer_t> layer_rects;
	QVector<QGraphicsLineItem*> gridLines;
	QVector<QLayoutDistanceMeasure*> distance_errors;
	QMap<QString,QLayoutMacroItem*> m_macroTemplateMap;
	QMap<QString,QLayoutViaItem*> m_viaTemplateMap;

	DRCSettings *drcDialog;
	QLayoutCellManager *cellManagerDialog;
};

#endif // QLAYOUTSCENE_H
