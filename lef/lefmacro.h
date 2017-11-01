#ifndef LEFMACRO_H
#define LEFMACRO_H

#include "lefpin.h"
#include "lefobstruction.h"

namespace lef {
	class LEFMacro {
	public:
		LEFMacro(QString);
		void addPin(QString);

		void setSize(qreal w, qreal h);
		void scaleMacro(qreal w, qreal h);
		double getWidth();
		double getHeight();

		QVector<QString> getPinNames();
		QVector<LEFPin*> getPins();
		LEFPin *getPin(QString name);
		QString getName();
		LEFObstruction *getObstruction();
		bool pinExists(QString n);

	private:
		QString name;
		QVector<LEFPin*> pins;
		LEFObstruction* obstructions;

		qreal sizeW;
		qreal sizeH;
	};
}

#endif // LEFMACRO_H
