#include "schematicssymbol.h"

namespace symbol {
	SchematicsSymbol::SchematicsSymbol(QString n, QString p) :
		m_box_x1(0),
		m_box_x2(0),
		m_box_y1(0),
		m_box_y2(0)
	{
		m_name = n;
		m_prefix = p;
	}

	QString SchematicsSymbol::getName()
	{
		return m_name;
	}

	QString SchematicsSymbol::getPrefix()
	{
		return m_prefix;
	}

	void SchematicsSymbol::addRect(int x1, int y1, int x2, int y2)
	{
		m_box_x1 = x1;
		m_box_x2 = x2;
		m_box_y1 = y1;
		m_box_y2 = y2;
	}

	void SchematicsSymbol::addPin(std::string name, int index, int x, int y, int length, std::string orient, int tw, int th, int a, int b, std::string mode)
	{
		m_pins.append(new SymbolPin(name, index, x, y, length, orient, tw, th, a, b, mode));
	}

	QRectF SchematicsSymbol::createRect()
	{
		return QRectF(m_box_x1, m_box_y1, m_box_x2-m_box_x1, m_box_y2-m_box_y1);
	}

	QVector<QSchematicsPin*> SchematicsSymbol::createPins(QGraphicsItem* p)
	{
		QVector<QSchematicsPin*> ret;

		ret.clear();
		foreach(SymbolPin* sp, m_pins) {
			QSchematicsPin* o = sp->createSchematicsPin(p);
			if(o) ret.append(o);
		}

		return ret;
	}
}
