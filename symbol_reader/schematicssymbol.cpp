#include "schematicssymbol.h"

namespace symbol {
	SchematicsSymbol::SchematicsSymbol(QString n, QString p)
	{
		m_name = n;
		m_prefix = p;
	}

	QString SchematicsSymbol::getName()
	{
		return m_name;
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

	QGraphicsRectItem* SchematicsSymbol::createRect(QGraphicsItem* p)
	{
		return new QGraphicsRectItem(m_box_x1, m_box_y1, m_box_x2-m_box_x1, m_box_y2-m_box_y1, p);
	}

	QVector<QSchematicsPin*> SchematicsSymbol::createPins(QGraphicsItem*p)
	{
		QVector<QSchematicsPin*> ret;
		QSchematicsPin *qp;

		return ret;
	}
}
