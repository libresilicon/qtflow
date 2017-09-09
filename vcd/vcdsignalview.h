#ifndef VCDSIGNALVIEW_H
#define VCDSIGNALVIEW_H

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

#include "vcd/vcd_data.hpp"

#define RAISE_TIME 10
#define BUS_VALUE_SPACING 5

class VcdSignalView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit VcdSignalView(QWidget *parent = nullptr);
	void setVCD(vcd::VcdData vcd_data);
	void append(QString);

signals:

protected:
	void drawSignal(QString signal_name, int idx);
	void drawSignalBus(QString signal_name, int idx);
	void drawTimeScale();
	void redraw();
	void rescale();

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

	void resizeEvent(QResizeEvent *event);

private:
	QVector<QString> signalViewFilter;
	QGraphicsScene *signalScene;
	vcd::VcdData vcd_data;
	QMap<QString,vcd::Var::Id> mapIdName;
	int lowest_time;
	int highest_time;
	int timeScale;
	int recentZeroTime;

	int moveDragLastX;
};

#endif // VCDSIGNALVIEW_H
