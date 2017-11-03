#ifndef DEFDATAPIN_H
#define DEFDATAPIN_H

#include <QString>

namespace def {
	class DEFDataPin
	{
	public:
		DEFDataPin();
		QString m_name;
		QString m_signal;
		QString m_layer;
		qreal m_x, m_y;
		qreal m_w, m_h;
	};
}

#endif // DEFDATAPIN_H
