%{
#include <cstdlib>
#include <iostream>

#include <QString>

#include "schematicsscanner.h"

void schematics_error(const char *s);
#define YY_DECL int schematics::SchematicsScanner::schematicslex( \
	schematics::SchematicsParser::semantic_type* schematicslval, \
	schematics::SchematicsData *schematicsdata)
%}

%top {
#include "schematics_parser.hpp"
#include "schematicsdata.h"
}

%option prefix="schematics"
%option c++
%option batch
%option stack
%option debug
%option verbose
%option pointer
%option yywrap
%option nounput
%option yylineno

STRING				[A-Za-z]|[~A-Za-z0-9_,.\-<>\[\]\/\(\)$\*\'=#?~\"]

BEGIN_SCHEMATIC		"EESchema"
HASH				"#"

LIBS				"LIBS"
EELAYER				"EELAYER"

DESCR				"$Descr"
DESCR_ENC			"encoding"
DESCR_SHEET			"Sheet"
DESCR_TITLE			"Title"
DESCR_DATE			"Date"
DESCR_REVISION		"Rev"
DESCR_COMP			"Comp"
DESCR_COMMENT		"Comment"
END_DESCR			"$EndDescr"

COMPONENT			"$Comp"
COMPONENT_L			"L"
COMPONENT_U			"U"
COMPONENT_P			"P"
COMPONENT_F			"F"
END_COMPONENT		"$EndComp"

TEXT				"Text"
WIRE				"Wire"
CONNECTION			"Connection"

END_SCHEMATIC		"$EndSCHEMATC"

%x component
%x descr

%%

{HASH}.*						{}

{BEGIN_SCHEMATIC}.*				{
	return schematics::SchematicsParser::token::BEGIN_SCHEMATIC;
}

{LIBS}[:]+							{
	return schematics::SchematicsParser::token::LIBS;
}

{EELAYER}+							{
	return schematics::SchematicsParser::token::EELAYER;
}

{DESCR}+						{
	BEGIN(descr);
	return schematics::SchematicsParser::token::DESCR;
}

<descr>{DESCR_ENC}+				{
	return schematics::SchematicsParser::token::DESCR_ENC;
}

<descr>{DESCR_SHEET}+			{
	return schematics::SchematicsParser::token::DESCR_SHEET;
}

<descr>{DESCR_TITLE}+			{
	return schematics::SchematicsParser::token::DESCR_TITLE;
}

<descr>{DESCR_DATE}+			{
	return schematics::SchematicsParser::token::DESCR_DATE;
}

<descr>{DESCR_REVISION}+		{
	return schematics::SchematicsParser::token::DESCR_REVISION;
}

<descr>{DESCR_COMP}+			{
	return schematics::SchematicsParser::token::DESCR_COMP;
}

<descr>{DESCR_COMMENT}[0-9]+	{
	return schematics::SchematicsParser::token::DESCR_COMMENT;
}

<descr>{END_DESCR}+				{
	BEGIN(INITIAL);
	return schematics::SchematicsParser::token::END_DESCR;
}

<descr>-[0-9]+|[0-9]+			{
	schematicslval->v_int = atoi(yytext);
	return schematics::SchematicsParser::token::INTEGER;
}

<descr>[0-9]+"."[0-9]*			{
	schematicslval->v_double = atof(yytext);
	return schematics::SchematicsParser::token::DOUBLE;
}

<descr>{STRING}*				{
	schematicslval->v_str = new std::string(yytext, yyleng);
	return schematics::SchematicsParser::token::STRING;
}

<descr>[ \n\t\r]+				{}

{COMPONENT}+					{
	BEGIN(component);
	return schematics::SchematicsParser::token::COMPONENT;
}

<component>{COMPONENT_L}		{
	return schematics::SchematicsParser::token::COMPONENT_L;
}

<component>{COMPONENT_U}		{
	return schematics::SchematicsParser::token::COMPONENT_U;
}

<component>{COMPONENT_P}		{
	return schematics::SchematicsParser::token::COMPONENT_P;
}

<component>{COMPONENT_F}		{
	return schematics::SchematicsParser::token::COMPONENT_F;
}

<component>{END_COMPONENT}+		{
	BEGIN(INITIAL);
	return schematics::SchematicsParser::token::END_COMPONENT;
}

<component>-[0-9]+|[0-9]+		{
	schematicslval->v_int = atoi(yytext);
	return schematics::SchematicsParser::token::INTEGER;
}

<component>[0-9]+"."[0-9]*		{
	schematicslval->v_double = atof(yytext);
	return schematics::SchematicsParser::token::DOUBLE;
}

<component>{STRING}*			{
	schematicslval->v_str = new std::string(yytext, yyleng);
	return schematics::SchematicsParser::token::STRING;
}

<component>[ \n\t\r]+			{}

{TEXT}+							{ return schematics::SchematicsParser::token::TEXT; }
{WIRE}+							{ return schematics::SchematicsParser::token::WIRE; }
{CONNECTION}+					{ return schematics::SchematicsParser::token::CONNECTION; }

{END_SCHEMATIC}+				{ return schematics::SchematicsParser::token::END_SCHEMATIC; }

-[0-9]+|[0-9]+					{
	schematicslval->v_int = atoi(yytext);
	return schematics::SchematicsParser::token::INTEGER;
}

[0-9]+"."[0-9]*					{
	schematicslval->v_double = atof(yytext);
	return schematics::SchematicsParser::token::DOUBLE;
}

{STRING}*						{
	schematicslval->v_str = new std::string(yytext, yyleng);
	return schematics::SchematicsParser::token::STRING;
}

[ \n\t\r]+						{}

%%
