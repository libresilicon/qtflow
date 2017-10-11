#ifndef SCHEMATICSSYMBOL_H
#define SCHEMATICSSYMBOL_H

#include <QString>

class SchematicsSymbol
{
public:
	SchematicsSymbol(QString n, QString p);
	QString getName();

private:
	QString name;
	QString prefix;
};

#endif // SCHEMATICSSYMBOL_H
