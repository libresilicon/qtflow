#ifndef SCHEMATICSDATA_H
#define SCHEMATICSDATA_H

namespace schematics {
	class SchematicsScanner;
	class SchematicsData
	{
	public:
		SchematicsData();
		class SchematicsScanner *getLexer();

	private:
		SchematicsScanner *lexer;
	};
}

#endif // SCHEMATICSDATA_H
