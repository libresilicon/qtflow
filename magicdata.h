#ifndef MAGICDATA_H
#define MAGICDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>

#include "magicscanner.h"

typedef QVector<QRect> rects_t;
typedef QMap<QString,rects_t> layer_rects_t;

typedef struct {
	QString module_name;
	QString instance_name;
	int x1,y1,x2,y2;
	int a,b,c,d,e,f;
} module_info_buffer;

typedef struct {
	QString module_name;
	QString instance_name;
	QRect box;
} module_info;
typedef QVector<module_info> mods_t;

namespace magic {
	class MagicData
	{
	public:
		MagicData();
		MagicData(QString file);

		bool loadFile(QString);

		layer_rects_t getRectangles();
		layer_rects_t getBoxes();
		mods_t getModules();

		// building the info
		void addRectangle(int,int,int,int);
		void addBox(int,int,int,int);
		void addUsedModuleBox(int,int,int,int);
		void addUsedModuleNames(std::string *module, std::string *name);
		void addUsedModuleTransform(int, int, int, int, int, int);
		void addUsedModule();
		void setLayer(std::string *s);
		class MagicScanner *getLexer();

	private:
		layer_rects_t parsedRectangles;
		layer_rects_t parsedBoxes;
		mods_t parsedModules;

		class MagicScanner* lexer;
		class MagicParser* parser;
		QString recentTitle;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;
		module_info_buffer recent_module;
	};
}

#endif // MAGICDATA_H
