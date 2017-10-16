#ifndef VCDVIEWGRAPHICSITEMBUS_H
#define VCDVIEWGRAPHICSITEMBUS_H

#include "vcdviewgraphicsitem.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QDebug>

#define RAISE_TIME 10

class VcdViewGraphicsItemBus : public VcdViewGraphicsItem
{
	Q_OBJECT

public:
	explicit VcdViewGraphicsItemBus(vcd::Var var, vcd::TimeBusValues values, QGraphicsItem *parent = nullptr);

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // VCDVIEWGRAPHICSITEMBUS_H
