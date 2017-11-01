#include "leflayer.h"

namespace lef {
	QString LEFLayer::getName()
	{
		return name;
	}

	QVector<rect_t> LEFLayer::getRects()
	{
		return rectsExport;
	}

	LEFLayer::LEFLayer(QString n) :
		scaleX(1),
		scaleY(1),
		name(n)
	{
	}

	void LEFLayer::addRectangle(double x, double y, double w, double h)
	{
		rect_t obj;
		obj.x = x;
		obj.y = y;
		obj.w = w;
		obj.h = h;
		rects.append(obj);
	}

	void LEFLayer::generateExportLayers()
	{
		rectsExport.clear();
		foreach(rect_t obj, rects) {
			rect_t nobj;
			nobj.x = obj.x*scaleX;
			nobj.y = obj.y*scaleY;
			nobj.w = obj.w*scaleX;
			nobj.h = obj.h*scaleY;
			rectsExport.append(nobj);
		}
	}

	void LEFLayer::scaleLayer(qreal w, qreal h)
	{
		scaleX=w;
		scaleY=h;
		generateExportLayers();
	}
}
