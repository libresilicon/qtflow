#include "lefdata.h"
#include "lefscanner.h"

namespace lef {
	LEFObstruction::LEFObstruction()
	{
	}

	bool LEFObstruction::layerExists(QString n)
	{
		foreach(lef::LEFLayer *layer, layers)
			if(layer->getName()==n)
				return true;
		return false;
	}

	QVector<LEFLayer*> LEFObstruction::getLayers()
	{
		return layers;
	}

	void LEFObstruction::addLayer(QString n)
	{
		layers.append(new LEFLayer(n));
	}

	LEFLayer *LEFObstruction::getLayer(QString n)
	{
		LEFLayer *ret = NULL;
		foreach(LEFLayer *l, layers)
			if(l->getName()==n)
				ret=l;
		return ret;
	}

	QString LEFLayer::getName()
	{
		return name;
	}

	QVector<rect_t> LEFLayer::getRects()
	{
		return rectsExport;
	}
	
	LEFLayer::LEFLayer(QString n) :
		scaleX(1),
		scaleY(1),
		name(n)
	{
	}
	
	void LEFLayer::addRectangle(double x, double y, double w, double h)
	{
		rect_t obj;
		obj.x = x;
		obj.y = y;
		obj.w = w;
		obj.h = h;
		rects.append(obj);
	}

	void LEFLayer::generateExportLayers()
	{
		rectsExport.clear();
		foreach(rect_t obj, rects) {
			rect_t nobj;
			nobj.x = obj.x*scaleX;
			nobj.y = obj.y*scaleY;
			nobj.w = obj.w*scaleX;
			nobj.h = obj.h*scaleY;
			rectsExport.append(nobj);
		}
	}

	void LEFLayer::scaleLayer(double w, double h)
	{
		scaleX=w;
		scaleY=h;
		generateExportLayers();
	}

	LEFPort::LEFPort()
	{
	}
	
	LEFLayer* LEFPort::getLayer(QString n)
	{
		LEFLayer *ret = NULL;
		foreach(LEFLayer *l, layers)
			if(l->getName()==n)
				ret=l;
		return ret;
	}
	
	void LEFPort::addLayer(QString n)
	{
		layers.append(new LEFLayer(n));
	}

	bool LEFPort::layerExists(QString n)
	{
		foreach(lef::LEFLayer *layer, layers)
			if(layer->getName()==n)
				return true;
		return false;
	}

	void LEFPort::scalePort(double w, double h)
	{
		foreach(LEFLayer* layer, layers) {
		    layer->scaleLayer(w,h);
		}
	}

	QVector<QString> LEFPort::getLayerNames()
	{
		QVector<QString> ret;
		foreach(LEFLayer *l, layers) ret.append(l->getName());
		return ret;
	}

	QVector<LEFLayer*> LEFPort::getLayers()
	{
		return layers;
	}

	LEFPort* LEFPin::getPort()
	{
		return port;
	}

	LEFPin::LEFPin(QString n) :
		port(new LEFPort())
	{
		name = n;
	}

	void LEFPin::scalePin(double w, double h)
	{
		port->scalePort(w,h);
	}

	QString LEFPin::getName()
	{
		return name;
	}

	QVector<LEFLayer*> LEFPin::getPortLayers()
	{
		return port->getLayers();
	}

	LEFMacro::LEFMacro(QString n) :
		sizeW(0),
		sizeH(0),
		name(n),
		obstructions(new LEFObstruction)
	{
	}

	QString LEFMacro::getName()
	{
		return name;
	}

	void LEFMacro::addPin(QString n)
	{
		pins.append(new LEFPin(n));
	}

	bool LEFMacro::pinExists(QString n)
	{
		foreach(lef::LEFPin *pin, pins)
			if(pin->getName()==n)
				return true;
		return false;
	}

	QVector<LEFPin*> LEFMacro::getPins()
	{
		return pins;
	}

	LEFPin* LEFMacro::getPin(QString name)
	{
		LEFPin *p = NULL;
		foreach(p, pins)
			if(p->getName()==name)
				return p;
		return p;
	}

	QVector<QString> LEFMacro::getPinNames()
	{
		QVector<QString> ret;
		foreach(LEFPin *m, pins) ret.append(m->getName());
		return ret;
	}

	LEFData::LEFData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false),
		baseUnitMicrons(false),
		baseUnitMicronsValue(1)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		lexer = new LEFScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new LEFParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	LEFData::LEFData() :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false),
		baseUnitMicrons(false),
		baseUnitMicronsValue(1)
	{
	}

	void LEFData::saveFile(QString filename)
	{
		std::ofstream output;
		std::string stdfilename = filename.toStdString();

		output.open(stdfilename, std::ios::out);
		output.close();
	}

	void LEFData::loadFile(QString filename)
	{
		std::ifstream input;
		std::string stdfilename = filename.toStdString();

		streamname = filename;
		input.open(stdfilename, std::ios::in);

		if(lexer) delete lexer;
		lexer = new LEFScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		if(parser) delete parser;
		parser = new LEFParser(this);
		parser->set_debug_level(trace_parsing);
		parser->parse();

		input.close();
	}

	bool LEFData::isDefinedMacro(QString name)
	{
		foreach(LEFMacro *m, macros)
			if(m->getName()==name)
				return true;
		return false;
	}

	LEFMacro* LEFData::getMacro(QString n)
	{
		LEFMacro *ret = NULL;
		foreach(LEFMacro *m, macros)
			if(m->getName()==n)
				ret = m;
		return ret;
	}

	LEFScanner *LEFData::getLexer()
	{
		return lexer;
	}

	void LEFData::storeMacro()
	{
		macros.append(recentMacro);
	}

	void LEFMacro::setSize(double w, double h)
	{
		sizeW = w;
		sizeH = h;
	}

	double LEFMacro::getWidth()
	{
		return sizeW;
	}

	double LEFMacro::getHeight()
	{
		return sizeH;
	}

	void LEFMacro::scaleMacro(int w, int h)
	{
		double scaleW = (1000*w/sizeW)/1000;
		double scaleH = (1000*h/sizeH)/1000;
		foreach(LEFPin *pin, pins) {
			pin->scalePin(scaleW,scaleH);
		}
		foreach(LEFLayer *layer, obstructions->getLayers()) {
			layer->scaleLayer(scaleW,scaleH);
		}
	}

	LEFObstruction *LEFMacro::getObstruction()
	{
		return obstructions;
	}

	void LEFData::setMacroSize(double w, double h)
	{
		recentMacro->setSize(w,h);
	}

	void LEFData::addMacroName(std::string *s)
	{
		recentMacro = new LEFMacro(QString::fromStdString(*s));
	}

	void LEFData::addMacroPinName(std::string *s)
	{
		recentMacroPinName = QString::fromStdString(*s);
		recentMacro->addPin(recentMacroPinName);
	}

	void LEFData::addMacroPinPortRectangle(double x1, double y1, double x2, double y2)
	{
		double x = x1;
		double y = y1;
		double w = x2-x1;
		double h = y2-y1;

		lef::LEFPin *pin;
		lef::LEFPort *port;
		lef::LEFLayer *layer;

		if(!recentMacro->pinExists(recentMacroPinName))
			recentMacro->addPin(recentMacroPinName);

		pin = recentMacro->getPin(recentMacroPinName);
		port = pin->getPort();

		if(!port->layerExists(recentMacroPinPortLayer))
			port->addLayer(recentMacroPinPortLayer);

		layer = port->getLayer(recentMacroPinPortLayer);
		layer->addRectangle(x, y, w, h);
	}

	void LEFData::addMacroPinPortLayer(std::string *s)
	{
		lef::LEFPin *pin;
		lef::LEFPort *port;

		recentMacroPinPortLayer = QString::fromStdString(*s);
		if(!recentMacro->pinExists(recentMacroPinName))
			recentMacro->addPin(recentMacroPinName);

		pin = recentMacro->getPin(recentMacroPinName);
		port = pin->getPort();
		if(!port->layerExists(recentMacroPinPortLayer))
			port->addLayer(recentMacroPinPortLayer);
	}

	void LEFData::addMacroPinObstructionLayer(std::string *s)
	{
		lef::LEFPin *pin;
		lef::LEFPort *port;

		recentMacroPinObstructionLayer = QString::fromStdString(*s);
	}

	void LEFData::addMacroPinObstructionRectangle(double x1, double y1, double x2, double y2)
	{
		double x = x1;
		double y = y1;
		double w = x2-x1;
		double h = y2-y1;

		lef::LEFObstruction *obstruction;
		lef::LEFLayer *layer;
		obstruction = recentMacro->getObstruction();
		if(!obstruction->layerExists(recentMacroPinPortLayer))
			obstruction->addLayer(recentMacroPinPortLayer);

		layer = obstruction->getLayer(recentMacroPinPortLayer);
		layer->addRectangle(x, y, w, h);
	}

	void LEFData::setBaseUnitMicrons(int i)
	{
		baseUnitMicrons = true;
		baseUnitMicronsValue = i;
	}
}
