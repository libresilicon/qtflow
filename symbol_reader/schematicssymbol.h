#ifndef SCHEMATICSSYMBOL_H
#define SCHEMATICSSYMBOL_H

#include <QString>

class SchematicsSymbol
{
public:
	SchematicsSymbol();
	QString getName();

private:
	QString name;
};

#endif // SCHEMATICSSYMBOL_H
