#ifndef LEFVIA_H
#define LEFVIA_H

#include <QString>
#include "leflayer.h"

namespace lef {
	class LEFVia
	{
	public:
		LEFVia(QString s);
		void addLayer(QString n);
		void addRect(double x1, double y1, double x2, double y2);

		qreal x();
		qreal y();
		qreal height();
		qreal width();

		QString getName();
		QVector<LEFLayer*> getLayers();

	private:
		QVector<LEFLayer*> m_layers;
		QString m_name;

		qreal m_x1;
		qreal m_y1;
		qreal m_x2;
		qreal m_y2;

		LEFLayer *m_recentLayer;
	};
}

#endif // LEFVIA_H
