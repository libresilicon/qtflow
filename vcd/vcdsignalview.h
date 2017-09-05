#ifndef VCDSIGNALVIEW_H
#define VCDSIGNALVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class VcdSignalView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit VcdSignalView(QWidget *parent = nullptr);

signals:

public slots:
	void mousePressEvent(QMouseEvent * e);

private:
	QGraphicsScene *signalScene;
};

#endif // VCDSIGNALVIEW_H
