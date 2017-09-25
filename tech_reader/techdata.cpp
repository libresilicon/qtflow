#include "tech_parser/tech_parser.h"
#include "techscanner.h"
#include "techdata.h"

namespace tech {
	TechData::TechData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new TechScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new TechParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	void TechData::setLayer(std::string *s)
	{
		recentTitle = QString::fromStdString(*s);
	}

	void TechData::setTechnology(std::string *s)
	{
		technology = QString::fromStdString(*s);
	}

	QString TechData::getTechnology()
	{
		return technology;
	}

	TechScanner *TechData::getLexer()
	{
		return lexer;
	}

	layer_rects_t TechData::getBoxes()
	{
		return parsedBoxes;
	}

	layer_rects_t TechData::getRectangles()
	{
		return parsedRectangles;
	}

	void TechData::addRectangle(int x1, int y1, int x2, int y2)
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
	}

	void TechData::addBox(int x1, int y1, int x2, int y2)
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
	}

	void TechData::addUsedModuleTransform(int a, int b, int c, int d, int e, int f)
	{
		recent_module.a = a;
		recent_module.b = b;
		recent_module.c = c;
		recent_module.d = d;
		recent_module.e = e;
		recent_module.f = f;
	}

	void TechData::addUsedModuleBox(int x1, int y1, int x2, int y2)
	{
		recent_module.x1 = x1;
		recent_module.y1 = y1;
		recent_module.x2 = x2;
		recent_module.y2 = y2;
	}

	void TechData::addUsedModuleNames(std::string *module, std::string *name)
	{
		recent_module.module_name = QString::fromStdString(*module);
		recent_module.instance_name = QString::fromStdString(*name);
	}

	void TechData::addUsedModule()
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

	mods_t TechData::getModules()
	{
		return parsedModules;
	}
}
