#include "lefobstruction.h"

namespace lef {
	LEFObstruction::LEFObstruction()
	{
	}

	bool LEFObstruction::layerExists(QString n)
	{
		foreach(lef::LEFLayer *layer, layers)
			if(layer->getName()==n)
				return true;
		return false;
	}

	QVector<LEFLayer*> LEFObstruction::getLayers()
	{
		return layers;
	}

	void LEFObstruction::addLayer(QString n)
	{
		layers.append(new LEFLayer(n));
	}

	LEFLayer* LEFObstruction::getLayer(QString n)
	{
		LEFLayer *ret = NULL;
		foreach(LEFLayer *l, layers)
			if(l->getName()==n)
				ret=l;
		return ret;
	}
}
