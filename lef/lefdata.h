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

	class LEFLayer {
	public:
		LEFLayer(QString);
		void addRectangle(double, double, double, double);
		void scaleLayer(double w, double h);
		QVector<rect_t> getRects();
		
		QString getName();
	private:
		QString name;
		QVector<rect_t> rects;
		QVector<rect_t> rectsExport;
		double scaleX;
		double scaleY;
		
		void generateExportLayers();
	};

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

	class LEFPort {
	public:
		LEFPort();
		QVector<QString> getLayerNames();
		QVector<LEFLayer*> getLayers();
		void scalePort(double,double);
		LEFLayer *getLayer(QString);
		bool layerExists(QString n);

		void addLayer(QString);
	private:
		QVector<LEFLayer*> layers;
	};

	class LEFPin {
	public:
		LEFPin(QString);
		void scalePin(double,double);

		QString getName();
		LEFPort *getPort();
		QVector<LEFLayer*> getPortLayers();
	private:
		QString name;
		LEFPort *port;
	};

	class LEFMacro {
	public:
		LEFMacro(QString);
		void addPin(QString);

		void setSize(double w, double h);
		void scaleMacro(int w, int h);

		QVector<QString> getPinNames();
		QVector<LEFPin*> getPins();
		LEFPin *getPin(QString name);
		QString getName();
		LEFObstruction *getObstruction();
		bool pinExists(QString n);
	private:
		QString name;
		QVector<LEFPin*> pins;
		LEFObstruction* obstructions;
		
		double sizeW;
		double sizeH;
	};

	class LEFScanner;
	class LEFParser;
	class LEFData
	{
	public:
		LEFData();
		LEFData(QString);
		void loadFile(QString);
		void saveFile(QString);
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
		void addMacroPinObstructionLayer(std::string *s);
		void addMacroPinObstructionRectangle(double, double, double, double);

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

		QString recentMacroPinObstructionLayer;

		bool baseUnitMicrons;
		int baseUnitMicronsValue;
	};
}

#endif // LEFDATA_H
