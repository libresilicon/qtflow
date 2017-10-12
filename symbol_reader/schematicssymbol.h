#ifndef SCHEMATICSSYMBOL_H
#define SCHEMATICSSYMBOL_H

#include <QString>
#include <QGraphicsRectItem>
#include <QGraphicsItem>

#include "qschematics/qschematicspin.h"

#include "symbolpin.h"

namespace symbol {
	class SchematicsSymbol
	{
	public:
		SchematicsSymbol(QString n, QString p);
		void addRect(int x1, int y1, int x2, int y2);
		void addPin(std::string name, int index, int x, int y, int length, std::string orient, int tw, int th, int a, int b, std::string mode);

		QGraphicsRectItem* createRect(QGraphicsItem*p);
		QVector<QSchematicsPin*> createPins(QGraphicsItem*p);

		QString getName();

	private:
		QString m_name;
		QString m_prefix;

		qreal m_box_x1;
		qreal m_box_x2;
		qreal m_box_y1;
		qreal m_box_y2;

		QVector<SymbolPin*> m_pins;
	};
}

#endif // SCHEMATICSSYMBOL_H
