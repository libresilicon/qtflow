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

namespace lef {
	typedef struct {
		QString name;
		QRect rect;
	} port_layer_t;

	class LEFPort {
	public:
		LEFPort();
		QVector<QString> getLayerNames();
		QVector<port_layer_t> getLayers();
		void addRectangleToLayer(QString, int x, int y, int w, int h);
	private:
		QVector<port_layer_t> *layers;
	};

	class LEFPin {
	public:
		LEFPin();
		LEFPin(QString);
		void addPortRectangleToLayer(QString, int, int, int, int);
		QString getName();
		LEFPort getPort();
		QVector<port_layer_t> getPortLayers();
	private:
		QString name;
		LEFPort port;
	};

	class LEFMacro {
	public:
		LEFMacro();
		LEFMacro(QString);
		void addPin(QString);

		QVector<QString> getPinNames();
		QVector<LEFPin> getPins();
		LEFPin getPin(QString name);
		QString getName();
	private:
		QString name;
		QVector<LEFPin> pins;
	};

	class LEFScanner;
	class LEFParser;
	class LEFData
	{
	public:
		LEFData(QString);
		LEFScanner *getLexer();

		bool isDefinedMacro(QString name);
		LEFMacro getMacro(QString);

		void setBaseUnitMicrons(int);

		void storeMacro();
		void addMacroName(std::string *s);
		void addMacroPinName(std::string *s);
		void addMacroPinPortLayer(std::string *s);
		void addMacroPinPortRectangle(double x1, double y1, double x2, double y2);

	private:
		LEFScanner *lexer;
		LEFParser* parser;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		// work variables:
		QVector<LEFMacro> macros;
		LEFMacro recentMacro;

		QString recentMacroName;
		QString recentMacroPinName;
		QString recentMacroPinPortLayer;

		bool baseUnitMicrons;
		int baseUnitMicronsValue;
	};
}

#endif // LEFDATA_H
