%{
#include <cstdlib>
#include <QString>

#include "blif/blifscanner.h"

#define YY_DECL int blif::BLIFScanner::bliflex(\
	blif::BLIFParser::semantic_type* bliflval,\
	blif::BLIFData *blifdata)
%}

%option prefix="blif"
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

STRING			[A-Za-z]|[A-Za-z0-9_,.-<>'"'\[\]\/$]

ASTERISK			"*"
PLUS				"+"
MINUS				"-"
SEMICOLON			";"
BRACKETOPEN			"("
BRACKETCLOSE		")"
COMMENT				"#"
VERSION				"VERSION"
NAMESCASESENSITIVE	"NAMESCASESENSITIVE"
BUSBITCHARS			"BUSBITCHARS"
DIVIDERCHAR			"DIVIDERCHAR"
UNITS				"UNITS"
END					"END"
DESIGN				"DESIGN"
MICRONS				"MICRONS"
USEMINSPACING		"USEMINSPACING"
DISTANCE			"DISTANCE"
DIEAREA				"DIEAREA"
COMPONENTS			"COMPONENTS"
TRACKS				"TRACKS"
DO					"DO"
LAYER				"LAYER"
STEP				"STEP"
PLACED				"PLACED"
NET					"NET"
PINS				"PINS"
NETS				"NETS"
ROUTED				"ROUTED"
NEW					"NEW"
SPECIALNETS			"SPECIALNETS"

%%
{SEMICOLON}.*					{}
{COMMENT}.*						{}
{VERSION}+						{ return blif::BLIFParser::token::VERSION; }
{NAMESCASESENSITIVE}+			{ return blif::BLIFParser::token::NAMESCASESENSITIVE; }
{BUSBITCHARS}+					{ return blif::BLIFParser::token::BUSBITCHARS; }
{DIVIDERCHAR}+					{ return blif::BLIFParser::token::DIVIDERCHAR; }
{UNITS}+						{ return blif::BLIFParser::token::UNITS; }
{END}+							{ return blif::BLIFParser::token::END; }
{DISTANCE}+						{ return blif::BLIFParser::token::DISTANCE; }
{MICRONS}+						{ return blif::BLIFParser::token::MICRONS; }
{DESIGN}+						{ return blif::BLIFParser::token::DESIGN; }
{BRACKETOPEN}+					{ return blif::BLIFParser::token::BRACKETOPEN; }
{BRACKETCLOSE}+					{ return blif::BLIFParser::token::BRACKETCLOSE; }
{DIEAREA}+						{ return blif::BLIFParser::token::DIEAREA; }
{COMPONENTS}+					{ return blif::BLIFParser::token::COMPONENTS; }
{TRACKS}+						{ return blif::BLIFParser::token::TRACKS; }
{DO}+							{ return blif::BLIFParser::token::DO; }
{LAYER}+						{ return blif::BLIFParser::token::LAYER; }
{STEP}+							{ return blif::BLIFParser::token::STEP; }
{MINUS}+						{ return blif::BLIFParser::token::MINUS; }
{PLUS}+							{ return blif::BLIFParser::token::PLUS; }
{PLACED}+						{ return blif::BLIFParser::token::PLACED; }
{NETS}+							{ return blif::BLIFParser::token::NETS; }
{NET}+							{ return blif::BLIFParser::token::NET; }
{PINS}+							{ return blif::BLIFParser::token::PINS; }
{ROUTED}+						{ return blif::BLIFParser::token::ROUTED; }
{ASTERISK}+						{ return blif::BLIFParser::token::ASTERISK; }
{NEW}+							{ return blif::BLIFParser::token::NEW; }
{SPECIALNETS}+					{ return blif::BLIFParser::token::SPECIALNETS; }

{INTEGER}* {
	bliflval->v_int = atoi(yytext);
	return blif::BLIFParser::token::INTEGER;
}

{DOUBLE}* {
	bliflval->v_double = atof(yytext);
	return blif::BLIFParser::token::DOUBLE;
}

{STRING}* {
	bliflval->v_str = new std::string(yytext, yyleng);
	return blif::BLIFParser::token::STRING;
}

[ \n\t\r]+	{}

%%
