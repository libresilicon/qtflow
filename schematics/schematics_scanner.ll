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

COMPONENT			"$Comp"
DESCR				"$Descr"

END_COMPONENT		"$EndComp"
END_DESCR			"$EndDescr"

%%

{DESCR}+			{ return schematics::SchematicsParser::token::DESCR; }
{END_DESCR}+		{ return schematics::SchematicsParser::token::END_DESCR; }

{COMPONENT}+		{ return schematics::SchematicsParser::token::COMPONENT; }
{END_COMPONENT}+	{ return schematics::SchematicsParser::token::END_COMPONENT; }

-[0-9]+|[0-9]+ {
	schematicslval->v_int = atoi(yytext);
	return schematics::SchematicsParser::token::INTEGER;
}

[0-9]+"."[0-9]* {
	schematicslval->v_double = atof(yytext);
	return schematics::SchematicsParser::token::DOUBLE;
}

[A-Za-z][A-Za-z0-9_,.-<>]* {
	schematicslval->v_str = new std::string(yytext, yyleng);
	return schematics::SchematicsParser::token::STRING;
}

%%
