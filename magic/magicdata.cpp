#include "magic_parser.hpp"
#include "magicscanner.h"
#include "magicdata.h"

#include <climits>

namespace magic {
	MagicData::MagicData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false),
		m_BBLowerX(0),
		m_BBLowerY(0),
		m_BBUpperX(0),
		m_BBUpperY(0)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new MagicScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new MagicParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	qreal MagicData::getLowerX()
	{
		return m_BBLowerX;
	}

	qreal MagicData::getLowerY()
	{
		return m_BBLowerY;
	}

	qreal MagicData::getUpperX()
	{
		return m_BBUpperX;
	}

	qreal MagicData::getUpperY()
	{
		return m_BBUpperY;
	}

	void MagicData::setLayer(std::string *s)
	{
		recentTitle = QString::fromStdString(*s);
	}

	void MagicData::setTechnology(std::string *s)
	{
		technology = QString::fromStdString(*s);
	}

	QString MagicData::getTechnology()
	{
		return technology;
	}

	MagicScanner *MagicData::getLexer()
	{
		return lexer;
	}

	layer_rects_t MagicData::getBoxes()
	{
		return parsedBoxes;
	}

	layer_rects_t MagicData::getRectangles()
	{
		return parsedRectangles;
	}

	void MagicData::setBoundaryRectangle(int x1, int y1, int x2, int y2)
	{
		if(m_BBLowerX>x1) m_BBLowerX = x1;
		if(m_BBLowerX>x2) m_BBLowerX = x2;

		if(m_BBLowerY>y1) m_BBLowerY = y1;
		if(m_BBLowerY>y2) m_BBLowerY = y2;

		if(m_BBUpperX<x1) m_BBUpperX = x1;
		if(m_BBUpperX<x2) m_BBUpperX = x2;

		if(m_BBUpperY<y1) m_BBUpperY = y1;
		if(m_BBUpperY<y2) m_BBUpperY = y2;
	}

	void MagicData::addRectangle(int x1, int y1, int x2, int y2)
	{
		rects_t list;
		rect_t objR;
		objR.x1 = x1;
		objR.x2 = x2;
		objR.y1 = y1;
		objR.y2 = y2;

		if(!parsedRectangles.contains(recentTitle)){
			parsedRectangles[recentTitle]=list;
		}
		parsedRectangles[recentTitle].append(objR);

		setBoundaryRectangle(x1, y1, x2, y2);
	}

	void MagicData::addBox(int x1, int y1, int x2, int y2)
	{
		rects_t list;
		rect_t objR;
		objR.x1 = x1;
		objR.x2 = x2;
		objR.y1 = y1;
		objR.y2 = y2;

		if(!parsedBoxes.contains(recentTitle)){
			parsedBoxes[recentTitle]=list;
		}
		parsedBoxes[recentTitle].append(objR);

		setBoundaryRectangle(x1, y1, x2, y2);
	}

	void MagicData::addUsedModuleTransform(int a, int b, int c, int d, int e, int f)
	{
		recent_module.a = a;
		recent_module.b = b;
		recent_module.c = c;
		recent_module.d = d;
		recent_module.e = e;
		recent_module.f = f;

		setBoundaryRectangle(c, f, c, f);
	}

	void MagicData::addUsedModuleBox(int x1, int y1, int x2, int y2)
	{
		recent_module.x1 = x1;
		recent_module.y1 = y1;
		recent_module.x2 = x2;
		recent_module.y2 = y2;

		setBoundaryRectangle(x1, y1, x2, y2);
	}

	void MagicData::addUsedModuleNames(std::string *module, std::string *name)
	{
		recent_module.module_name = QString::fromStdString(*module);
		recent_module.instance_name = QString::fromStdString(*name);
	}

	void MagicData::addUsedModule()
	{
		module_info info;
		info.a = recent_module.a;
		info.b = recent_module.b;
		info.c = recent_module.c;
		info.d = recent_module.d;
		info.e = recent_module.e;
		info.f = recent_module.f;

		info.x1 = recent_module.x1;
		info.x2 = recent_module.x2;

		info.y1 = recent_module.y1;
		info.y2 = recent_module.y2;

		info.module_name = recent_module.module_name;
		info.instance_name = recent_module.instance_name;

		parsedModules.append(info);
	}

	mods_t MagicData::getModules()
	{
		return parsedModules;
	}
}
