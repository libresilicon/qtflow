%{
#include <cstdlib>
#include <iostream>

#include <QString>

#include "schematics/schematicsscanner.h"

void schematics_error(const char *s);
#define YY_DECL int schematics::SchematicsScanner::schematicslex( \
	schematics::SchematicsParser::semantic_type* schematicslval, \
	schematics::SchematicsData *schematicsdata)
%}

%top {
#include "schematics_parser/schematics_parser.h"
#include "schematics/schematicsdata.h"
}

%option yywrap
%option nounput
%option prefix="schematics"

LIBS			"LIBS"
COMPONENT		"\$Comp"
END_COMPONENT	"\$EndComp"

%%

LIBS			{ return schematics::SchematicsParser::token::LIBS; }
COMPONENT		{ return schematics::SchematicsParser::token::COMPONENT; }
END_COMPONENT	{ return schematics::SchematicsParser::token::END_COMPONENT; }

%%
