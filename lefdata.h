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
		double x;
		double y;
		double w;
		double h;
	} rect_t;

	class LEFPortLayer {
	public:
		LEFPortLayer(QString);
		void addRectangle(double, double, double, double);
		void setOffsetX(double);
		void setOffsetY(double);
		void scaleLayer(double w, double h);
		QVector<QRect> getRects();
		
		QString getName();
	private:
		QString name;
		QVector<rect_t> rects;
		QVector<QRect> rectsExport;
		double offsetX;
		double offsetY;
		double scaleX;
		double scaleY;
		
		void generateExportLayers();
	};

	class LEFPort {
	public:
		LEFPort();
		QVector<QString> getLayerNames();
		QVector<LEFPortLayer*> getLayers();
		void scalePort(double,double);
		LEFPortLayer *getLayer(QString);
		bool layerExists(QString n);

		void addLayer(QString);
	private:
		QVector<LEFPortLayer*> layers;
	};

	class LEFPin {
	public:
		LEFPin();
		LEFPin(QString);
		void scalePin(double,double);

		QString getName();
		LEFPort *getPort();
		QVector<LEFPortLayer*> getPortLayers();
	private:
		QString name;
		LEFPort *port;
	};

	class LEFMacro {
	public:
		LEFMacro();
		LEFMacro(QString);
		void addPin(QString);
		void setSize(double w, double h);
		void scaleMacro(int w, int h);

		QVector<QString> getPinNames();
		QVector<LEFPin*> getPins();
		LEFPin *getPin(QString name);
		QString getName();
		bool pinExists(QString n);
	private:
		QString name;
		QVector<LEFPin*> pins;
		
		double sizeW;
		double sizeH;
	};

	class LEFScanner;
	class LEFParser;
	class LEFData
	{
	public:
		LEFData(QString);
		LEFScanner *getLexer();

		bool isDefinedMacro(QString name);
		LEFMacro *getMacro(QString);

		void setBaseUnitMicrons(int);

		void storeMacro();
		void setMacroSize(double w, double h);
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
		QVector<LEFMacro*> macros;
		LEFMacro *recentMacro;

		QString recentMacroName;
		QString recentMacroPinName;
		QString recentMacroPinPortLayer;

		bool baseUnitMicrons;
		int baseUnitMicronsValue;
	};
}

#endif // LEFDATA_H
