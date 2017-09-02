#include "magic_parser/magic_parser.h"
#include "magicscanner.h"
#include "magicdata.h"

namespace magic {
	MagicData::MagicData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
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

	void MagicData::addRectangle(int x1, int y1, int x2, int y2)
	{
		rects_t list;
		QRect objR(x1,y1,x2-x1,y2-y1);
		if(!parsedRectangles.contains(recentTitle)){
			parsedRectangles[recentTitle]=list;
		}
		parsedRectangles[recentTitle].append(objR);
	}

	void MagicData::addBox(int x1, int y1, int x2, int y2)
	{
		rects_t list;
		QRect objR(x1,y1,x2-x1,y2-y1);
		if(!parsedBoxes.contains(recentTitle)){
			parsedBoxes[recentTitle]=list;
		}
		parsedBoxes[recentTitle].append(objR);
	}

	void MagicData::addUsedModuleTransform(int a, int b, int c, int d, int e, int f)
	{
		recent_module.a = a;
		recent_module.b = b;
		recent_module.c = c;
		recent_module.d = d;
		recent_module.e = e;
		recent_module.f = f;
	}

	void MagicData::addUsedModuleBox(int x1, int y1, int x2, int y2)
	{
		recent_module.x1 = x1;
		recent_module.y1 = y1;
		recent_module.x2 = x2;
		recent_module.y2 = y2;
	}

	void MagicData::addUsedModuleNames(std::string *module, std::string *name)
	{
		recent_module.module_name = QString::fromStdString(*module);
		recent_module.instance_name = QString::fromStdString(*name);
	}

	void MagicData::addUsedModule()
	{
		module_info info;
		info.box = QRect(
				recent_module.c,
				recent_module.f,
				recent_module.a*(recent_module.x2-recent_module.x1),
				recent_module.e*(recent_module.y2-recent_module.y1)
				);

		info.xoffset = recent_module.c;
		info.yoffset = recent_module.f;

		info.xsign = recent_module.a;
		info.ysign = recent_module.e;

		info.module_name = new QGraphicsTextItem;
		info.module_name->setPlainText(recent_module.module_name);
		info.module_name->setPos(recent_module.c,recent_module.f);
		info.instance_name = new QGraphicsTextItem;
		info.instance_name->setPlainText(recent_module.instance_name);
		info.instance_name->setPos(recent_module.c,recent_module.f+recent_module.e*(recent_module.y2-recent_module.y1));
		info.module_name_plain = recent_module.module_name; // for looking up in LEF file

		parsedModules.append(info);
	}

	mods_t MagicData::getModules()
	{
		return parsedModules;
	}
}
