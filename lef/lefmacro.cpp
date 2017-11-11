#include "lefmacro.h"

namespace lef {
	LEFMacro::LEFMacro(QString n) :
		m_sizeW(0),
		m_sizeH(0),
		m_name(n),
		m_obstructions(new LEFObstruction)
	{
	}

	QString LEFMacro::getName()
	{
		return m_name;
	}

	void LEFMacro::addPin(QString n)
	{
		m_pins.append(new LEFPin(n));
	}

	bool LEFMacro::pinExists(QString n)
	{
		foreach(lef::LEFPin *pin, m_pins)
			if(pin->getName()==n)
				return true;
		return false;
	}

	QVector<LEFPin*> LEFMacro::getPins()
	{
		return m_pins;
	}

	LEFPin* LEFMacro::getPin(QString name)
	{
		LEFPin *p = NULL;
		foreach(p, m_pins)
			if(p->getName()==name)
				return p;
		return p;
	}

	QVector<QString> LEFMacro::getPinNames()
	{
		QVector<QString> ret;
		foreach(LEFPin *m, m_pins) ret.append(m->getName());
		return ret;
	}

	void LEFMacro::setSize(qreal w, qreal h)
	{
		m_sizeW = w;
		m_sizeH = h;
	}

	double LEFMacro::getWidth()
	{
		return m_sizeW;
	}

	double LEFMacro::getHeight()
	{
		return m_sizeH;
	}

	LEFObstruction *LEFMacro::getObstruction()
	{
		return m_obstructions;
	}

	QString LEFMacro::getClass()
	{
		return m_class;
	}

	void LEFMacro::setClass(QString s)
	{
		m_class = s;
	}

}
