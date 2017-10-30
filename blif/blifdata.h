#ifndef SBLIFDATA_H
#define SBLIFDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QFile>

#include "blifdatacomponentinfo.h"

namespace blif {
	enum blif_keyword {
		MODEL,
		INPUTS,
		OUTPUTS,
		NAMES,
		GATE,
		SUB_COMPONENT,
		END,
		UNKNOWN
	};

	class BLIFData
	{
	public:
		BLIFData(QString file);

		QVector<BLIFDataComponentInfo> getComponents();

	protected:
		blif_keyword tokenize(QString s);

		void registerInputPins(QStringList lineList);
		void registerOutputPins(QStringList lineList);
		void registerComponent(QStringList lineList);

	private:
		QString streamname;

		QString m_modelName;

		QStringList m_inputList;
		QStringList m_outputList;

		QVector<BLIFDataComponentInfo> m_components;

	};
}

#endif // SBLIFDATA_H
