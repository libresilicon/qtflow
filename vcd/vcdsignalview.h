#ifndef VCDSIGNALVIEW_H
#define VCDSIGNALVIEW_H

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "vcdviewgraphicsitemsignal.h"
#include "vcdviewgraphicsitembus.h"
#include "vcdviewgraphicsitemtimescale.h"

#include "vcd_reader/vcd_data.hpp"

#define RAISE_TIME 10

class SignalBusArea
{
public:
	SignalBusArea() : isUnfolded(false), bus_width(1) {}
	QRect area;
	bool isUnfolded;
	int bus_width;
};

class VcdSignalView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit VcdSignalView(QWidget *parent = nullptr);
	void setVCD(vcd::VcdData vcd_data);
	void append(QString);

signals:

protected:
	bool drawSignalBus(QString signal_name);
	bool drawSubSignals(QString signal_name);

	QString getHierarchyNameString(std::vector<std::string> l);

public slots:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

	void wheelEvent(QWheelEvent *event);

	void contextMenuEvent(QContextMenuEvent *event);

	void onZoomIn();
	void onZoomOut();
	void onZoomFitWidth();
	void onMoveRight();
	void onMoveLeft();

	void onRemoveSignal();
	void onUnFoldSignalBus();
	void onFoldSignalBus();

	void resizeEvent(QResizeEvent *event);

private:
	QString longSignalID(std::vector<std::string> arr);

	QStringList signalViewFilter;
	QGraphicsScene *signalScene;
	vcd::VcdData vcd_data;
	QMap<QString,vcd::Var::Id> mapIdName;
	int lowest_time;
	int highest_time;
	int timeScale;
	int recentZeroTime;

	QMap<QString,SignalBusArea> busSignalAreas;
	QMap<QString,QRect> signalAreas;

	int moveDragLastX;

	int drawingIndex;
};

#endif // VCDSIGNALVIEW_H
