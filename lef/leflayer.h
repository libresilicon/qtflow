#ifndef LEFLAYER_H
#define LEFLAYER_H

#include <QVector>
#include <QDebug>

namespace lef {
	typedef struct {
		qreal x;
		qreal y;
		qreal w;
		qreal h;
	} rect_t;

	class LEFLayer {
	public:
		LEFLayer(QString);
		void addRectangle(double, double, double, double);
		QVector<rect_t> getRects();

		QString getName();
	private:
		QString m_name;
		QVector<rect_t> m_rects;

		void generateExportLayers();
	};
}

#endif // LEFLAYER_H
