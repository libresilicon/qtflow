#include "lefport.h"

namespace lef {
	LEFPort::LEFPort()
	{
	}

	LEFLayer* LEFPort::getLayer(QString n)
	{
		LEFLayer *ret = NULL;
		foreach(LEFLayer *l, m_layers)
			if(l->getName()==n)
				ret=l;
		return ret;
	}

	void LEFPort::addLayer(QString n)
	{
		m_layers.append(new LEFLayer(n));
	}

	bool LEFPort::layerExists(QString n)
	{
		foreach(lef::LEFLayer *layer, m_layers)
			if(layer->getName()==n)
				return true;
		return false;
	}

	QVector<QString> LEFPort::getLayerNames()
	{
		QVector<QString> ret;
		foreach(LEFLayer *l, m_layers) ret.append(l->getName());
		return ret;
	}

	QVector<LEFLayer*> LEFPort::getLayers()
	{
		return m_layers;
	}
}
