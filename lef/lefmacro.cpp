#include "lefmacro.h"

namespace lef {
	LEFMacro::LEFMacro(QString n) :
		sizeW(0),
		sizeH(0),
		name(n),
		obstructions(new LEFObstruction)
	{
	}

	QString LEFMacro::getName()
	{
		return name;
	}

	void LEFMacro::addPin(QString n)
	{
		pins.append(new LEFPin(n));
	}

	bool LEFMacro::pinExists(QString n)
	{
		foreach(lef::LEFPin *pin, pins)
			if(pin->getName()==n)
				return true;
		return false;
	}

	QVector<LEFPin*> LEFMacro::getPins()
	{
		return pins;
	}

	LEFPin* LEFMacro::getPin(QString name)
	{
		LEFPin *p = NULL;
		foreach(p, pins)
			if(p->getName()==name)
				return p;
		return p;
	}

	QVector<QString> LEFMacro::getPinNames()
	{
		QVector<QString> ret;
		foreach(LEFPin *m, pins) ret.append(m->getName());
		return ret;
	}

	void LEFMacro::setSize(qreal w, qreal h)
	{
		sizeW = w;
		sizeH = h;
	}

	double LEFMacro::getWidth()
	{
		return sizeW;
	}

	double LEFMacro::getHeight()
	{
		return sizeH;
	}

	void LEFMacro::scaleMacro(qreal w, qreal h)
	{
		qreal scaleW = w/sizeW;
		qreal scaleH = h/sizeH;
		foreach(LEFPin *pin, pins) {
			pin->scalePin(scaleW,scaleH);
		}
		foreach(LEFLayer *layer, obstructions->getLayers()) {
			layer->scaleLayer(scaleW,scaleH);
		}
	}

	LEFObstruction *LEFMacro::getObstruction()
	{
		return obstructions;
	}

}
