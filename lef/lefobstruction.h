#ifndef LEFOBSTRUCTION_H
#define LEFOBSTRUCTION_H

#include <QVector>

#include "leflayer.h"

namespace lef {
class LEFObstruction {
public:
	LEFObstruction();
	void addLayer(QString);
	QVector<LEFLayer*> getLayers();
	bool layerExists(QString n);
	LEFLayer *getLayer(QString);
	void scaleObstruction(double,double);

private:
	QVector<LEFLayer*> layers;
};
}


#endif // LEFOBSTRUCTION_H
