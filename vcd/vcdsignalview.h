#ifndef VCDSIGNALVIEW_H
#define VCDSIGNALVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

#include "vcd/vcd_data.hpp"

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

public slots:
	void mousePressEvent(QMouseEvent * e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void contextMenuEvent(QContextMenuEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);

	void onZoomIn();
	void onZoomOut();

private:
	QVector<QString> signalViewFilter;
	QGraphicsScene *signalScene;
	vcd::VcdData vcd_data;
	QMap<QString,vcd::Var::Id> mapIdName;
	int lowest_time;
	int highest_time;
	int timeScale;
};

#endif // VCDSIGNALVIEW_H
