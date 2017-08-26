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
	int x,y,w,h;
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
		void addRectangle(int x, int y, int w, int h);
		void addBox(int x, int y, int w, int h);
		void addUsedModuleBox(int x, int y, int w, int h);
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
