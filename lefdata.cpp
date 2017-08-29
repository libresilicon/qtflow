#include "lefdata.h"
#include "lefscanner.h"

namespace lef {
	LEFPin::LEFPin() {}

	LEFPin::LEFPin(QString n)
	{
		name = n;
	}

	QString LEFPin::getName()
	{
		return name;
	}

	QVector<QRect> LEFPin::getPortRectangles(QString l)
	{
		return port[l];
	}

	QList<QString> LEFPin::getPortLayers()
	{
		return port.keys();
	}

	void LEFPin::addPortRectangleToLayer(QString l, int x1, int y1, int x2, int y2)
	{
		port[l].append(QRect(x1,y1,x2,y2));
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

	LEFPin LEFMacro::getPin(QString n)
	{
		foreach(LEFPin m, pins)
			if(m.getName()==n)
				return m;
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
		trace_parsing(false)
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
		recentMacro.getPin(recentMacroPinName).addPortRectangleToLayer(recentMacroPinPortLayer,x1,y1,x2,y2);
	}

	void LEFData::addMacroPinPortLayer(std::string *s)
	{
		recentMacroPinPortLayer = QString::fromStdString(*s);
	}
}

