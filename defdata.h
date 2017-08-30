#ifndef SDEFDATA_H
#define SDEFDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

typedef QVector<QRect> rects_t;
typedef QMap<QString,rects_t> layer_rects_t;

typedef struct {
	QString module_name;
	QString instance_name;
	int x1,y1,x2,y2;
	int a,b,c,d,e,f;
} module_info_buffer;

typedef struct {
	QString module_name_plain;
	QGraphicsTextItem *module_name;
	QGraphicsTextItem *instance_name;
	QRect box;
	double xoffset, yoffset;
	int xsign, ysign;
} module_info;

typedef QVector<module_info> mods_t;

namespace magic {
	class DEFScanner;
	class DEFData
	{
	public:
		DEFData();
		DEFData(QString file);

		bool loadFile(QString);

		layer_rects_t getRectangles();
		layer_rects_t getBoxes();
		mods_t getModules();
		QString getTechnology();

		// building the info
		void addRectangle(int,int,int,int);
		void addBox(int,int,int,int);
		void addUsedModuleBox(int,int,int,int);
		void addUsedModuleNames(std::string *module, std::string *name);
		void addUsedModuleTransform(int, int, int, int, int, int);
		void addUsedModule();
		void setLayer(std::string *s);
		void setTechnology(std::string *s);
		class DEFScanner *getLexer();

	private:
		layer_rects_t parsedRectangles;
		layer_rects_t parsedBoxes;
		mods_t parsedModules;

		class DEFScanner* lexer;
		class DEFParser* parser;
		QString recentTitle;
		QString technology;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;
		module_info_buffer recent_module;
	};
}

#endif // SDEFDATA_H
