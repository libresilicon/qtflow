%{
#include <cstdlib>
#include <QString>

#include "lefscanner.h"

#define YY_DECL int lef::LEFScanner::leflex(\
	lef::LEFParser::semantic_type* leflval,\
	lef::LEFData *lefdata)
%}

%option header-file="lef.ll.h"
%option prefix="lef"
%option c++
%option batch
%option stack
%option debug
%option verbose
%option pointer
%option yywrap
%option nounput
%option yylineno

NUMBER			[0-9]
INTEGER			-{NUMBER}+|{NUMBER}+
EXPONENT		[eE][+-]?{INTEGER}
DOUBLE			{INTEGER}("."{INTEGER})?{EXPONENT}?

STRING			[A-Za-z]|[A-Za-z0-9_,.-<>'"'\[\]\/]

SEMICOLON			";"
COMMENT				"#"
VERSION				"VERSION"
NAMESCASESENSITIVE	"NAMESCASESENSITIVE"
BUSBITCHARS			"BUSBITCHARS"
DIVIDERCHAR			"DIVIDERCHAR"
UNITS				"UNITS"
END					"END"
DATABASE			"DATABASE"
MICRONS				"MICRONS"
USEMINSPACING		"USEMINSPACING"
OBS					"OBS"
PIN					"PIN"
CLEARANCEMEASURE	"CLEARANCEMEASURE"
MANUFACTURINGGRID	"MANUFACTURINGGRID"

LAYER				"LAYER"
TYPE				"TYPE"
SPACING				"SPACING"
DIRECTION			"DIRECTION"
PITCH				"PITCH"
OFFSET				"OFFSET"
WIDTH				"WIDTH"
RESISTANCE			"RESISTANCE"
CAPACITANCE			"CAPACITANCE"

VIA					"VIA"
RECT				"RECT"

VIARULE				"VIARULE"
TO					"TO"
BY					"BY"
USE					"USE"

OVERHANG			"OVERHANG"
METALOVERHANG		"METALOVERHANG"

SITE				"SITE"
SYMMETRY			"SYMMETRY"
CLASS				"CLASS"
SIZE				"SIZE"

MACRO				"MACRO"
FOREIGN				"FOREIGN"
ORIGIN				"ORIGIN"
SHAPE				"SHAPE"
PORT				"PORT"

%%

{SEMICOLON}.*				{}
{COMMENT}.*					{}
{VERSION}+					{ return lef::LEFParser::token::VERSION; }
{NAMESCASESENSITIVE}+		{ return lef::LEFParser::token::NAMESCASESENSITIVE; }
{BUSBITCHARS}+				{ return lef::LEFParser::token::BUSBITCHARS; }
{DIVIDERCHAR}+				{ return lef::LEFParser::token::DIVIDERCHAR; }
{UNITS}+					{ return lef::LEFParser::token::UNITS; }
{END}+						{ return lef::LEFParser::token::END; }
{DATABASE}+					{ return lef::LEFParser::token::DATABASE; }
{MICRONS}+					{ return lef::LEFParser::token::MICRONS; }
{USEMINSPACING}+			{ return lef::LEFParser::token::USEMINSPACING; }
{OBS}+						{ return lef::LEFParser::token::OBS; }
{PIN}+						{ return lef::LEFParser::token::PIN; }
{CLEARANCEMEASURE}+			{ return lef::LEFParser::token::CLEARANCEMEASURE; }
{MANUFACTURINGGRID}+		{ return lef::LEFParser::token::MANUFACTURINGGRID; }
{LAYER}+					{ return lef::LEFParser::token::LAYER; }
{TYPE}+						{ return lef::LEFParser::token::TYPE; }
{SPACING}+					{ return lef::LEFParser::token::SPACING; }
{DIRECTION}+				{ return lef::LEFParser::token::DIRECTION; }
{PITCH}+					{ return lef::LEFParser::token::PITCH; }
{OFFSET}+					{ return lef::LEFParser::token::OFFSET; }
{WIDTH}+					{ return lef::LEFParser::token::WIDTH; }
{RESISTANCE}+				{ return lef::LEFParser::token::RESISTANCE; }
{CAPACITANCE}+				{ return lef::LEFParser::token::CAPACITANCE; }
{VIA}+						{ return lef::LEFParser::token::VIA; }
{RECT}+						{ return lef::LEFParser::token::RECT; }
{VIARULE}+					{ return lef::LEFParser::token::VIARULE; }
{TO}+						{ return lef::LEFParser::token::TO; }
{BY}+						{ return lef::LEFParser::token::BY; }
{OVERHANG}+					{ return lef::LEFParser::token::OVERHANG; }
{METALOVERHANG}+			{ return lef::LEFParser::token::METALOVERHANG; }
{SITE}+						{ return lef::LEFParser::token::SITE; }
{SYMMETRY}+					{ return lef::LEFParser::token::SYMMETRY; }
{CLASS}+					{ return lef::LEFParser::token::CLASS; }
{SIZE}+						{ return lef::LEFParser::token::SIZE; }
{MACRO}+					{ return lef::LEFParser::token::MACRO; }
{FOREIGN}+					{ return lef::LEFParser::token::FOREIGN; }
{ORIGIN}+					{ return lef::LEFParser::token::ORIGIN; }
{USE}+					{ return lef::LEFParser::token::USE; }
{SHAPE}+					{ return lef::LEFParser::token::SHAPE; }
{PORT}+					{ return lef::LEFParser::token::PORT; }

{INTEGER}* {
	leflval->v_int = atoi(yytext);
	return lef::LEFParser::token::INTEGER;
}

{DOUBLE}* {
	leflval->v_double = atof(yytext);
	return lef::LEFParser::token::DOUBLE;
}

{STRING}* {
	leflval->v_str = new std::string(yytext, yyleng);
	return lef::LEFParser::token::STRING;
}

[ \n\t\r]+ {
	/* yylloc->step(); */
}
