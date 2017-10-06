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
#include <QGraphicsTextItem>


typedef struct {
	int x1,y1,x2,y2;
} rect_t;

typedef QVector<rect_t> rects_t;
typedef QMap<QString,rects_t> layer_rects_t;

typedef struct {
	QString module_name;
	QString instance_name;
	int x1,y1,x2,y2;
	int a,b,c,d,e,f;
} module_info;

typedef QVector<module_info> mods_t;

namespace magic {
	class MagicScanner;
	class MagicData
	{
	public:
		MagicData(QString file);

		layer_rects_t getRectangles();
		layer_rects_t getBoxes();
		mods_t getModules();
		QString getTechnology();

		// get bbox values
		qreal getLowerX();
		qreal getLowerY();
		qreal getUpperX();
		qreal getUpperY();

		// building the info
		void addRectangle(int,int,int,int);
		void addBox(int,int,int,int);
		void addUsedModuleBox(int,int,int,int);
		void addUsedModuleNames(std::string *module, std::string *name);
		void addUsedModuleTransform(int, int, int, int, int, int);
		void addUsedModule();
		void setLayer(std::string *s);
		void setTechnology(std::string *s);
		class MagicScanner *getLexer();

	private:
		void setBoundaryRectangle(int, int, int, int);

		layer_rects_t parsedRectangles;
		layer_rects_t parsedBoxes;
		mods_t parsedModules;

		class MagicScanner* lexer;
		class MagicParser* parser;
		QString recentTitle;
		QString technology;
		bool trace_parsing;
		bool trace_scanning;
		QString streamname;
		module_info recent_module;

		// bounding rectangle
		int m_BBLowerX;
		int m_BBLowerY;
		int m_BBUpperX;
		int m_BBUpperY;
	};
}

#endif // MAGICDATA_H
