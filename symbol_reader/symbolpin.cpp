#include "symbolpin.h"

namespace symbol {
	SymbolPin::SymbolPin(std::string name, int index, int x, int y, int length, std::string orient, int tw, int th, int a, int b, std::string mode)
	{
		m_name = QString::fromStdString(name);
		m_orient = QString::fromStdString(orient);
		m_mode = QString::fromStdString(mode);

		m_index = index;
		m_x = x;
		m_y = y;
		m_length = length;
		m_text_width = tw;
		m_text_heigth = th;
		m_a = a;
		m_b = b;
	}

	QSchematicsPin* SymbolPin::createSchematicsPin(QGraphicsItem* p)
	{
		return new QSchematicsPin(m_name, m_index, m_x, m_y, m_length, m_orient, m_text_width, m_text_heigth, m_a, m_b, m_mode, p);
	}
}
