#ifndef VCDSIGNALVIEW_H
#define VCDSIGNALVIEW_H

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QGraphicsLinearLayout>

#include "vcdviewgraphicsitemsignal.h"
#include "vcdviewgraphicsitembus.h"
#include "vcdviewgraphicsitemtimescale.h"

#include "vcd_reader/vcd_data.hpp"

#define RAISE_TIME 10

class VcdSignalView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit VcdSignalView(QWidget *parent = nullptr);
	void setVCD(vcd::VcdData vcd_data);
	void append(QString);

signals:

protected:
	bool drawSubSignals(QString signal_name);

	QString getHierarchyNameString(std::vector<std::string> l);

public slots:
	void onZoomIn();
	void onZoomOut();
	void onZoomFitWidth();

	void onRemoveSignal(VcdViewGraphicsItem *m);

private:
	QString longSignalID(std::vector<std::string> arr);

	QStringList signalViewFilter;
	QGraphicsScene *signalScene;
	vcd::VcdData vcd_data;

	int m_lowest_time;
	int m_highest_time;

	QMap<QString,QRect> signalAreas;
	QVector<VcdViewGraphicsItem*> m_signals;

	int moveDragLastX;

	int drawingIndex;
};

#endif // VCDSIGNALVIEW_H
