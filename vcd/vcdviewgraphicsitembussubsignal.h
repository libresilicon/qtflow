#ifndef VCDVIEWGRAPHICSITEMBUSSUBSIGNAL_H
#define VCDVIEWGRAPHICSITEMBUSSUBSIGNAL_H

#include "vcdviewgraphicsitem.h"

class VcdViewGraphicsItemBusSubSignal : public VcdViewGraphicsItem
{
public:
	explicit VcdViewGraphicsItemBusSubSignal(vcd::Var var, vcd::TimeBusValues values, QGraphicsItem *parent = nullptr);

};

#endif // VCDVIEWGRAPHICSITEMBUSSUBSIGNAL_H
