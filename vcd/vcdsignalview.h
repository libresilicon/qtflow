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
	void drawTimeScale();

public slots:
	void mousePressEvent(QMouseEvent * e);

private:
	QVector<QString> signalViewFilter;
	QGraphicsScene *signalScene;
	vcd::VcdData vcd_data;
	QMap<QString,vcd::Var::Id> mapIdName;
	int lowest_time;
	int highest_time;
};

#endif // VCDSIGNALVIEW_H
