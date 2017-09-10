#ifndef VCDSIGNALVIEW_H
#define VCDSIGNALVIEW_H

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

#include "vcd/vcd_data.hpp"

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
	bool drawSignal(QString signal_name);
	bool drawSignalBus(QString signal_name);
	bool drawSubSignals(QString signal_name);
	void drawTimeScale();
	void redraw();
	void rescale();

	QString getHierarchyNameString(std::vector<std::string> l);

public slots:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

	void wheelEvent(QWheelEvent *event);

	void contextMenuEvent(QContextMenuEvent *event);

	void dragEnterEvent(QDragEnterEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *e);
	void dragMoveEvent(QDragMoveEvent *e);
	void dropEvent(QDropEvent *event);

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
