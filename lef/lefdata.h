#ifndef LEFDATA_H
#define LEFDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

#include "lefmacro.h"
#include "lefvia.h"
#include "leflayerinfo.h"

namespace lef {

	class LEFScanner;
	class LEFParser;
	class LEFData
	{
	public:
		LEFData(QString);
		void saveFile(QString);
		LEFScanner *getLexer();

		bool isDefinedMacro(QString name);
		lef::LEFMacro *getMacro(QString);
		QVector<LEFMacro*> getMacros();
		QVector<LEFVia*> getVias();

		void setBaseUnitMicrons(int);
		int getBaseUnits();

		bool isDefinedVia(QString name);
		void addViaName(std::string s);
		void addViaLayer(std::string s);
		void addViaLayerRectangle(double x1, double y1, double x2, double y2);

		void setMacroSize(double w, double h);
		void addMacroName(std::string s);
		void addMacroPinName(std::string s);
		void addMacroPinPortLayer(std::string s);
		void addMacroPinPortRectangle(double x1, double y1, double x2, double y2);
		void addMacroPinObstructionLayer(std::string s);
		void addMacroPinObstructionRectangle(double, double, double, double);
		void setMacroClass(std::string s);
		void setSubBitChar(std::string s);
		void setDivideChar(std::string s);
		QString getSubBitChar();
		QString getDivideChar();

		void addLayer(std::string s);
		void setLayerType(std::string s);
		void setLayerPitch(double);
		QVector<LEFLayerInfo*> getLayers();

	private:
		LEFScanner *lexer;
		LEFParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		// work variables:
		QVector<LEFMacro*> m_macros;
		QVector<LEFVia*> m_vias;
		LEFMacro *m_recentMacro;
		LEFVia *m_recentVia;
		LEFLayerInfo *m_recentLayer;
		QVector<LEFLayerInfo*> m_layers;

		QString recentMacroName;
		QString recentMacroPinName;
		QString recentMacroPinPortLayer;
		QString m_subBitChar;
		QString m_divideChar;

		QString recentMacroPinObstructionLayer;

		bool baseUnitMicrons;
		int m_baseUnitMicronsValue;
	};
}

#endif // LEFDATA_H
