#ifndef SYMBOLPIN_H
#define SYMBOLPIN_H

#include <string>
#include <QString>

namespace symbol {
	class SymbolPin
	{
	public:
		SymbolPin(std::string name, int index, int x, int y, int length, std::string orient, int tw, int th, int a, int b, std::string mode);

	private:
		QString m_name;
		QString m_orient;
		QString m_mode;

		int m_index;
		int m_x;
		int m_y;
		int m_length;
		int m_text_width;
		int m_text_heigth;
		int m_a;
		int m_b;
	};
}

#endif // SYMBOLPIN_H
