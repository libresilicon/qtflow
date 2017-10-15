#ifndef VCDVIEWGRAPHICSITEMSIGNAL_H
#define VCDVIEWGRAPHICSITEMSIGNAL_H

#include <QObject>
#include <QWidget>

class VcdViewGraphicsItemSignal : public QWidget
{
	Q_OBJECT
public:
	explicit VcdViewGraphicsItemSignal(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // VCDVIEWGRAPHICSITEMSIGNAL_H