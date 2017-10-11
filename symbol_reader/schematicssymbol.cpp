#include "schematicssymbol.h"

SchematicsSymbol::SchematicsSymbol(QString n, QString p)
{
	name = n;
	prefix = p;
}

QString SchematicsSymbol::getName()
{
	return name;
}
