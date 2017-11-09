#ifndef LEFLAYER_H
#define LEFLAYER_H

#include <QVector>
#include <QDebug>

namespace lef {
	typedef struct {
		double x;
		double y;
		double w;
		double h;
	} rect_t;

	class LEFLayer {
	public:
		LEFLayer(QString);
		void addRectangle(double, double, double, double);
		void scaleLayer(qreal w, qreal h);
		QVector<rect_t> getRects();

		QString getName();
	private:
		QString name;
		QVector<rect_t> rects;
		QVector<rect_t> rectsExport;
		qreal scaleX;
		qreal scaleY;

		void generateExportLayers();
	};
}

#endif // LEFLAYER_H
