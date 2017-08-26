#include "magicdata.h"

namespace magic {
	MagicData::MagicData():
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
	}

	MagicData::MagicData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		std::cout << "opening file " << stdfilename << std::endl;
		input.open(stdfilename, std::ios::in);
		std::cout << "opened file" << stdfilename << std::endl;

		lexer = new MagicScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new MagicParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	bool MagicData::loadFile(QString filename)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();
		int stat;

		if(filename==QString())
			return false;

		streamname = filename;

		std::cout << "opening file " << stdfilename << std::endl;
		input.open(stdfilename, std::ios::in);
		std::cout << "opened file" << stdfilename << std::endl;

		if(lexer) delete lexer;
		lexer = new MagicScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		if(parser) delete parser;
		parser = new MagicParser(this);
		parser->set_debug_level(trace_parsing);

		stat=parser->parse();
		input.close();

		return stat;
	}

	void MagicData::setLayer(std::string *s)
	{
		recentTitle = QString::fromStdString(*s);
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

	void MagicData::addRectangle(int x, int y, int w, int h)
	{
		rects_t list;
		QRect objR(x,y,w,h);
		if(!parsedRectangles.contains(recentTitle)){
			parsedRectangles[recentTitle]=list;
		}
		parsedRectangles[recentTitle].append(objR);
	}

	void MagicData::addBox(int x, int y, int w, int h)
	{
		rects_t list;
		QRect objR(x,y,w,h);
		if(!parsedBoxes.contains(recentTitle)){
			parsedBoxes[recentTitle]=list;
		}
		parsedBoxes[recentTitle].append(objR);
	}

	void MagicData::addUsedModuleTransform(int a, int b, int c, int d, int e, int f)
	{
		recent_module.x = c;
		recent_module.y = f;
	}

	void MagicData::addUsedModuleBox(int x, int y, int w, int h)
	{
		//recent_module.x = x;
		//recent_module.y = y;
		recent_module.w = w;
		recent_module.h = h;
	}

	void MagicData::addUsedModuleNames(std::string *module, std::string *name)
	{
		recent_module.module_name = QString::fromStdString(*module);
		recent_module.instance_name = QString::fromStdString(*name);
	}

	void MagicData::addUsedModule()
	{
		module_info info;
		info.box = QRect(recent_module.x,recent_module.y,recent_module.w,recent_module.h);
		info.module_name = recent_module.module_name;
		info.instance_name = recent_module.instance_name;
		parsedModules.append(info);
	}

	mods_t MagicData::getModules()
	{
		return parsedModules;
	}

}
