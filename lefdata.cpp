#include "lefdata.h"
#include "lefscanner.h"

namespace lef {
	LEFPort::LEFPort() :
		layers(new QVector<port_layer_t>)
	{
	}

	void LEFPort::addRectangleToLayer(QString l, int x, int y, int w, int h)
	{
		port_layer_t n;
		n.name=l;
		n.rect=QRect(x,y,w,h);
		layers->append(n);
	}

	QVector<QString> LEFPort::getLayerNames()
	{
		QVector<QString> ret;
		foreach(port_layer_t l, *layers) ret.append(l.name);
		return ret;
	}

	QVector<port_layer_t> LEFPort::getLayers()
	{
		return *layers;
	}

	LEFPin::LEFPin() {}

	LEFPort LEFPin::getPort()
	{
		return port;
	}

	LEFPin::LEFPin(QString n)
	{
		name = n;
	}

	QString LEFPin::getName()
	{
		return name;
	}

	QVector<port_layer_t> LEFPin::getPortLayers()
	{
		return port.getLayers();
	}

	void LEFPin::addPortRectangleToLayer(QString l, int x, int y, int w, int h)
	{
		port.addRectangleToLayer(l,x,y,w,h);
	}

	LEFMacro::LEFMacro() {}

	LEFMacro::LEFMacro(QString n)
	{
		name = n;
	}

	QString LEFMacro::getName()
	{
		return name;
	}

	void LEFMacro::addPin(QString n)
	{
		pins.append(LEFPin(n));
	}

	QVector<LEFPin> LEFMacro::getPins()
	{
		return pins;
	}

	LEFPin LEFMacro::getPin(QString name)
	{
		LEFPin p;
		foreach(p, pins)
			if(p.getName()==name)
				return p;
		return p;
	}

	QVector<QString> LEFMacro::getPinNames()
	{
		QVector<QString> ret;
		foreach(LEFPin m, pins) ret.append(m.getName());
		return ret;
	}

	LEFData::LEFData(QString filename) :
		lexer(NULL),
		parser(NULL),
		trace_scanning(false),
		trace_parsing(false),
		baseUnitMicrons(false)
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

	bool LEFData::isDefinedMacro(QString name)
	{
		foreach(LEFMacro m, macros)
			if(m.getName()==name)
				return true;
		return false;
	}

	LEFMacro LEFData::getMacro(QString n)
	{
		foreach(LEFMacro m, macros)
			if(m.getName()==n)
				return m;
	}

	LEFScanner *LEFData::getLexer()
	{
		return lexer;
	}

	void LEFData::storeMacro()
	{
		macros.append(recentMacro);
	}

	void LEFData::addMacroName(std::string *s)
	{
		recentMacro = LEFMacro(QString::fromStdString(*s));
	}

	void LEFData::addMacroPinName(std::string *s)
	{
		recentMacro.addPin(QString::fromStdString(*s));
		recentMacroPinName = QString::fromStdString(*s);
	}

	void LEFData::addMacroPinPortRectangle(double x1, double y1, double x2, double y2)
	{
		int x = (int)x1;
		int y = (int)y1;
		int w = (int)x2 - (int)x1;
		int h = (int)y2 - (int)y1;

		/*if(baseUnitMicrons) {
			x/=baseUnitMicronsValue;
			y/=baseUnitMicronsValue;
			w/=baseUnitMicronsValue;
			h/=baseUnitMicronsValue;
		}*/

		recentMacro.getPin(recentMacroPinName).addPortRectangleToLayer(recentMacroPinPortLayer, x, y, w, h);
	}

	void LEFData::addMacroPinPortLayer(std::string *s)
	{
		recentMacroPinPortLayer = QString::fromStdString(*s);
	}

	void LEFData::setBaseUnitMicrons(int i)
	{
		baseUnitMicrons = true;
		baseUnitMicronsValue = i;
	}

}

