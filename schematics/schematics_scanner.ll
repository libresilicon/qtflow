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
%option yylineno
%option nounput
%option prefix="schematics"

STRING				[A-Za-z]|[~A-Za-z0-9_,.\-<>\[\]\/\(\)$\*\'=#?~\"]

COMPONENT			"$Comp"
DESCR				"$Descr"

END_COMPONENT		"$EndComp"
END_DESCR			"$EndDescr"

TEXT				"Text"
WIRE				"Wire"
CONNECTION			"Connection"

END_SCHEMATIC		"$EndSCHEMATC"

COMPONENT_L			"L"
COMPONENT_U			"U"
COMPONENT_P			"P"
COMPONENT_F			"F"

%x component

%%

{DESCR}+			{ return schematics::SchematicsParser::token::DESCR; }
{END_DESCR}+		{ return schematics::SchematicsParser::token::END_DESCR; }

{COMPONENT}+		{
	BEGIN(component);
	return schematics::SchematicsParser::token::COMPONENT;
}

<component>{COMPONENT_L}	{
	return schematics::SchematicsParser::token::COMPONENT_L;
}

<component>{COMPONENT_U}	{
	return schematics::SchematicsParser::token::COMPONENT_U;
}

<component>{COMPONENT_P}	{
	return schematics::SchematicsParser::token::COMPONENT_P;
}

<component>{COMPONENT_F}	{
	return schematics::SchematicsParser::token::COMPONENT_F;
}

<component>{END_COMPONENT}+	{
	BEGIN(INITIAL);
	return schematics::SchematicsParser::token::END_COMPONENT;
}

<component>-[0-9]+|[0-9]+			{
	schematicslval->v_int = atoi(yytext);
	return schematics::SchematicsParser::token::INTEGER;
}

<component>[0-9]+"."[0-9]*			{
	schematicslval->v_double = atof(yytext);
	return schematics::SchematicsParser::token::DOUBLE;
}

<component>{STRING}*				{
	schematicslval->v_str = new std::string(yytext, yyleng);
	return schematics::SchematicsParser::token::STRING;
}

<component>[ \n\t\r]+		{}

{TEXT}+				{ return schematics::SchematicsParser::token::TEXT; }
{WIRE}+				{ return schematics::SchematicsParser::token::WIRE; }
{CONNECTION}+		{ return schematics::SchematicsParser::token::CONNECTION; }

{END_SCHEMATIC}+	{ return schematics::SchematicsParser::token::END_SCHEMATIC; }

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
