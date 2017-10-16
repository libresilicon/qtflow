#ifndef VCDVIEWGRAPHICSITEMSIGNAL_H
#define VCDVIEWGRAPHICSITEMSIGNAL_H

#include "vcdviewgraphicsitem.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

class VcdViewGraphicsItemSignal : public VcdViewGraphicsItem
{
public:
	explicit VcdViewGraphicsItemSignal(vcd::Var var, vcd::TimeValues values, QGraphicsItem *parent = nullptr);

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	bool m_isFirstValue;
};

#endif // VCDVIEWGRAPHICSITEMSIGNAL_H
