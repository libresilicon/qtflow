#include "schematicsdata.h"
#include "schematicsscanner.h"

namespace schematics {
	SchematicsData::SchematicsData(QString filename) :
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

		lexer = new SchematicsScanner(&input, &std::cout);
		lexer->set_debug(trace_scanning);

		parser = new SchematicsParser(this);
		//parser->set_debug_level(trace_parsing);
		parser->parse();
		input.close();
	}

	SchematicsScanner *SchematicsData::getLexer()
	{
		return lexer;
	}

	void SchematicsData::addWire(std::string name, int x1, int y1, int x2, int y2)
	{
		SchematicsWire wire = SchematicsWire(QString::fromStdString(name),x1,y1,x2,y2);
		wires.append(wire);
		setBoundaryRectangle(x1,y1,x2,y2);
	}

	QVector<SchematicsWire> SchematicsData::getWires()
	{
		return wires;
	}

	void SchematicsData::setBoundaryRectangle(int x1, int y1, int x2, int y2)
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

	qreal SchematicsData::getLowerX()
	{
		return m_BBLowerX;
	}

	qreal SchematicsData::getLowerY()
	{
		return m_BBLowerY;
	}

	qreal SchematicsData::getUpperX()
	{
		return m_BBUpperX;
	}

	qreal SchematicsData::getUpperY()
	{
		return m_BBUpperY;
	}


}
