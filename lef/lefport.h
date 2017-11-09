#ifndef LEFPORT_H
#define LEFPORT_H

#include <QVector>

#include "leflayer.h"

namespace lef {
	class LEFPort {
	public:
		LEFPort();
		QVector<QString> getLayerNames();
		QVector<LEFLayer*> getLayers();
		LEFLayer *getLayer(QString);
		bool layerExists(QString n);

		void addLayer(QString);
	private:
		QVector<LEFLayer*> m_layers;

	};
}

#endif // LEFPORT_H
