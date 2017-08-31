%{
#include <cstdlib>
#include <QString>

#include "defscanner.h"

#define YY_DECL int def::DEFScanner::deflex(\
	def::DEFParser::semantic_type* deflval,\
	def::DEFData *defdata)
%}

%option header-file="def.ll.h"
%option prefix="def"
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
DESIGN				"DESIGN"
SYMMETRY			"SYMMETRY"
CLASS				"CLASS"
SIZE				"SIZE"
MACRO				"MACRO"
FOREIGN				"FOREIGN"
ORIGIN				"ORIGIN"
SHAPE				"SHAPE"
PORT				"PORT"
LIBRARY				"LIBRARY"
DISTANCE				"DISTANCE"

%%
{SEMICOLON}.*				{}
{COMMENT}.*					{}
{VERSION}+					{ return def::DEFParser::token::VERSION; }
{NAMESCASESENSITIVE}+		{ return def::DEFParser::token::NAMESCASESENSITIVE; }
{BUSBITCHARS}+				{ return def::DEFParser::token::BUSBITCHARS; }
{DIVIDERCHAR}+				{ return def::DEFParser::token::DIVIDERCHAR; }
{UNITS}+					{ return def::DEFParser::token::UNITS; }
{END}+						{ return def::DEFParser::token::END; }
{DISTANCE}+						{ return def::DEFParser::token::DISTANCE; }
{MICRONS}+						{ return def::DEFParser::token::MICRONS; }
{DESIGN}+						{ return def::DEFParser::token::DESIGN; }

{INTEGER}* {
	deflval->v_int = atoi(yytext);
	return def::DEFParser::token::INTEGER;
}

{DOUBLE}* {
	deflval->v_double = atof(yytext);
	return def::DEFParser::token::DOUBLE;
}

{STRING}* {
	deflval->v_str = new std::string(yytext, yyleng);
	return def::DEFParser::token::STRING;
}

[ \n\t\r]+ {
	/* yylloc->step(); */
}

%%
