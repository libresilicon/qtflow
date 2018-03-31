#ifndef QLAYOUTSCENE_H
#define QLAYOUTSCENE_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QRubberBand>
#include <QGraphicsProxyWidget>
#include <QShortcut>

#include "qlayoutdistancemeasure.h"
#include "qlayoutrectitem.h"
#include "qlayoutmacroitem.h"
#include "qlayoutviaitem.h"
#include "qlayout/tech_data_wrapper.hpp"

#include "qlayoutcellmanager.h"
#include "drcsettings.h"
#include "lef/lefdata.h"
#include "gds2/gdtdata.h"
#include "tech_reader/techdesignrule.h"

enum drawing_operations {
	DRAWING_OPERATION_NONE,
	DRAWING_OPERATION_SELECT,
	DRAWING_OPERATION_SELECT_LAYER,
	DRAWING_OPERATION_RECTANGLE,
	DRAWING_OPERATION_POLYGON,
	DRAWING_OPERATION_CUT_OUT,
	DRAWING_OPERATION_DRAG,
	DRAWING_OPERATION_DIMENSION
};

#define ALL_LAYERS "all layers"
#define LAYOUT_SCALE_FACTOR 100

typedef QVector<QLayoutRectItem*> rects_layer_t;

class QLayoutScene : public QGraphicsScene
{
	Q_OBJECT
public:
	QLayoutScene(QObject *parent = Q_NULLPTR);
	QLayoutScene(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
	QLayoutScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);
	void basicInit();

	void showDRC();
	void showCellManager();

	void setDrawingOperation(drawing_operations o);

	void addVia(QString netname, QString vianame, QPointF p);
	void addWire(QString netname, QString layer, QPointF p1, QPointF p2);
	void addRectangle(QString layer, qreal x, qreal y, qreal w, qreal h);
	void addPad(QString name, QString net, QString layer, qreal x, qreal y, qreal w, qreal h);
	void addMacro(QString module_name, QString instance_name, qreal x, qreal y, qreal w, qreal h, QString orient);
	void addMacro(QString macro_name, QString instance_name, qreal x, qreal y, QString orient);

	void setLambdaUnit(QString s);
	void setLambdaValue(qreal i);
	void setTechData(TechDataWrapper*t);

	QStringList getLayers();
	QVector<QLayoutRectItem*> getRectangles(QString n);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	void keyPressEvent(QKeyEvent *event);

	void drawBackground(QPainter *painter, const QRectF &rect);

public slots:
	void setGDS(QString name);
	void setLEF(QString name);
	void onVisibleLayersChanged(QStringList l);
	void runDRC();
	void setActiveLayer(QString layer);

signals:
	void contentChanged();
	void contentSaved();

	void registerLayer(QString);

private:
	void refreshMacroTable();
	void refreshViaTable();
	void runDRC(QString n, QRectF rect);

	QPointF snapGrid(QPointF pt);
	int countSelectedRectItems(QVector<QLayoutRectItem*> l);

	QString m_activeLayer;
	QLayoutRectItem *m_recentRectangle;
	QGraphicsRectItem *m_recentSelectRectangle;
	QLayoutDistanceMeasure *m_recentDistanceMeasure;
	drawing_operations m_recentOperation;
	QPointF m_lastOrig;
	QStringList m_visibleLayers;
	bool m_dragging;
	TechDataWrapper* m_techData;

	QVector<QLayoutMacroItem*> macros;
	QVector<QGraphicsTextItem*> macro_texts;
	//QMap<QString,layer_macro_wires_t> macro_wires;
	QMap<QString,rects_layer_t> layer_rects;
	QVector<QGraphicsLineItem*> gridLines;
	QVector<QLayoutDistanceMeasure*> distance_errors;
	QMap<QString,QLayoutMacroItem*> m_macroTemplateMap;
	QMap<QString,QLayoutViaItem*> m_viaTemplateMap;
	int m_gridSize;
	QString m_lambaUnit;
	qreal m_lambaValue;

	DRCSettings *drcDialog;
	QLayoutCellManager *cellManagerDialog;
};

#endif // QLAYOUTSCENE_H
