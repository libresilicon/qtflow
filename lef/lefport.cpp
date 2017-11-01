#include "lefport.h"

namespace lef {
	LEFPort::LEFPort()
	{
	}

	LEFLayer* LEFPort::getLayer(QString n)
	{
		LEFLayer *ret = NULL;
		foreach(LEFLayer *l, layers)
			if(l->getName()==n)
				ret=l;
		return ret;
	}

	void LEFPort::addLayer(QString n)
	{
		layers.append(new LEFLayer(n));
	}

	bool LEFPort::layerExists(QString n)
	{
		foreach(lef::LEFLayer *layer, layers)
			if(layer->getName()==n)
				return true;
		return false;
	}

	void LEFPort::scalePort(qreal w, qreal h)
	{
		foreach(LEFLayer* layer, layers) {
			layer->scaleLayer(w,h);
		}
	}

	QVector<QString> LEFPort::getLayerNames()
	{
		QVector<QString> ret;
		foreach(LEFLayer *l, layers) ret.append(l->getName());
		return ret;
	}

	QVector<LEFLayer*> LEFPort::getLayers()
	{
		return layers;
	}
}
