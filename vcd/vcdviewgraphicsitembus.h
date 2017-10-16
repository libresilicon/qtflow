#ifndef VCDVIEWGRAPHICSITEMBUS_H
#define VCDVIEWGRAPHICSITEMBUS_H

#include "vcdviewgraphicsitem.h"

#define RAISE_TIME 10

class VcdViewGraphicsItemBus : public VcdViewGraphicsItem
{
public:
	explicit VcdViewGraphicsItemBus(vcd::Var var, vcd::TimeBusValues values, QGraphicsItem *parent = nullptr);
};

#endif // VCDVIEWGRAPHICSITEMBUS_H
