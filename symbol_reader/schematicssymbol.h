#ifndef SCHEMATICSSYMBOL_H
#define SCHEMATICSSYMBOL_H

#include <QString>
#include <QGraphicsRectItem>
#include <QGraphicsItem>

namespace symbol {
	class SchematicsSymbol
	{
	public:
		SchematicsSymbol(QString n, QString p);
		void addRect(int x1, int y1, int x2, int y2);
		QGraphicsRectItem *createRect(QGraphicsItem*p);

		QString getName();

	private:
		QString m_name;
		QString m_prefix;

		qreal m_box_x1;
		qreal m_box_x2;
		qreal m_box_y1;
		qreal m_box_y2;
	};
}

#endif // SCHEMATICSSYMBOL_H
