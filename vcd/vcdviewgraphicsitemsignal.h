#ifndef VCDVIEWGRAPHICSITEMSIGNAL_H
#define VCDVIEWGRAPHICSITEMSIGNAL_H

#include "vcdviewgraphicsitem.h"

class VcdViewGraphicsItemSignal : public VcdViewGraphicsItem
{
public:
	explicit VcdViewGraphicsItemSignal(vcd::Var var, vcd::TimeValues values, QGraphicsItem *parent = nullptr);
private:
	bool m_isFirstValue;
};

#endif // VCDVIEWGRAPHICSITEMSIGNAL_H
