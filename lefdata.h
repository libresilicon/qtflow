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
	class LEFPin {
	public:
		LEFPin();
		LEFPin(QString);
		void addPortRectangleToLayer(QString, int x1, int y1, int x2, int y2);
		QString getName();
		QList<QString> getPortLayers();
		QVector<QRect> getPortRectangles(QString l);
	private:
		QString name;
		QMap<QString,QVector<QRect>> port;
	};

	class LEFMacro {
	public:
		LEFMacro();
		LEFMacro(QString);
		void addPin(QString);

		QVector<QString> getPinNames();
		LEFPin getPin(QString);
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
	};
}

#endif // LEFDATA_H
