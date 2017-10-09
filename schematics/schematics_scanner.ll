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

STRING				[A-Za-z]|[~A-Za-z0-9_,.\-<>\[\]\/\(\)$\*\'=#?~]

COMPONENT			"$Comp"
DESCR				"$Descr"

END_COMPONENT		"$EndComp"
END_DESCR			"$EndDescr"

TEXT				"Text"
WIRE				"Wire"
CONNECTION			"Connection"

END_SCHEMATIC		"$EndSCHEMATC"

%x multiline

%%

{DESCR}+			{ return schematics::SchematicsParser::token::DESCR; }
{END_DESCR}+		{ return schematics::SchematicsParser::token::END_DESCR; }

{COMPONENT}+		{ return schematics::SchematicsParser::token::COMPONENT; }
{END_COMPONENT}+	{ return schematics::SchematicsParser::token::END_COMPONENT; }

{TEXT}+				{ return schematics::SchematicsParser::token::TEXT; }
{WIRE}+				{ return schematics::SchematicsParser::token::WIRE; }
{CONNECTION}+		{ return schematics::SchematicsParser::token::CONNECTION; }

{END_SCHEMATIC}+	{ return schematics::SchematicsParser::token::END_SCHEMATIC; }

[\"]+					{
	BEGIN(multiline);
	return schematics::SchematicsParser::token::Multiline;
}

<multiline>.*			{
		const char* end;
		for(int i = YYLeng();i>0;i--) {
			end = YYText() + i;
			if(*end) {
				if(*end=='\\') {
					BEGIN(multiline);
				} else {
					BEGIN(INITIAL);
				}
				break;
			}
		}
	return schematics::SchematicsParser::token::Multiline;
}

-[0-9]+|[0-9]+			{
	schematicslval->v_int = atoi(yytext);
	return schematics::SchematicsParser::token::INTEGER;
}

[0-9]+"."[0-9]*			{
	schematicslval->v_double = atof(yytext);
	return schematics::SchematicsParser::token::DOUBLE;
}

{STRING}*				{
	schematicslval->v_str = new std::string(yytext, yyleng);
	return schematics::SchematicsParser::token::STRING;
}

[ \n\t\r]+				{}

%%
