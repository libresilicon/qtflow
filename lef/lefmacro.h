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
		qreal getWidth();
		qreal getHeight();

		QVector<QString> getPinNames();
		QVector<LEFPin*> getPins();
		LEFPin *getPin(QString name);
		QString getName();
		QString getClass();
		QString getSite();
		void setClass(QString s);
		void setSite(QString s);
		LEFObstruction *getObstruction();
		bool pinExists(QString n);

	private:
		QString m_name;
		QString m_class;
		QString m_site;
		QVector<LEFPin*> m_pins;
		LEFObstruction* m_obstructions;

		qreal m_sizeW;
		qreal m_sizeH;
	};
}

#endif // LEFMACRO_H
