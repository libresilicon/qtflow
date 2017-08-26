%{
#include <cstdlib>
#include <iostream>

#include <QString>

#include "schematicsdata.h"

void schematics_error(const char *s);
#define YY_DECL int schematics::SchematicsScanner::schematicslex( \
	schematics::SchematicsParser::semantic_type* schematicslval, \
	schematics::SchematicsData *schematicsdata)
%}

%option header-file="schematics.ll.h"
%option yywrap
%option nounput
%option stack
%option prefix="schematics"

LIBS			"LIBS"
COMPONENT		"\$Comp"
END_COMPONENT	"\$EndComp"
%%

LIBS			{ return schematics::SchematicsParser::token::LIBS; }
COMPONENT		{ return schematics::SchematicsParser::token::COMPONENT; }
END_COMPONENT	{ return schematics::SchematicsParser::token::END_COMPONENT; }

%%
