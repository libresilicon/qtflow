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

STRING			[A-Za-z]|[A-Za-z0-9_,.-<>'"'\[\]\/$]

PLUS			"+"
MINUS			"-"
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
DESIGN					"DESIGN"
MICRONS				"MICRONS"
USEMINSPACING		"USEMINSPACING"
DISTANCE				"DISTANCE"
DIEAREA				"DIEAREA"
COMPONENTS				"COMPONENTS"
TRACKS				"TRACKS"
DO				"DO"
LAYER				"LAYER"
STEP				"STEP"
PLACED				"PLACED"
NET				"NET"
PINS				"PINS"
NETS				"NETS"

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
{BRACKETOPEN}+						{ return def::DEFParser::token::BRACKETOPEN; }
{BRACKETCLOSE}+						{ return def::DEFParser::token::BRACKETCLOSE; }
{DIEAREA}+						{ return def::DEFParser::token::DIEAREA; }
{COMPONENTS}+						{ return def::DEFParser::token::COMPONENTS; }
{TRACKS}+						{ return def::DEFParser::token::TRACKS; }
{DO}+						{ return def::DEFParser::token::DO; }
{LAYER}+						{ return def::DEFParser::token::LAYER; }
{STEP}+						{ return def::DEFParser::token::STEP; }
{MINUS}+						{ return def::DEFParser::token::MINUS; }
{PLUS}+						{ return def::DEFParser::token::PLUS; }
{PLACED}+						{ return def::DEFParser::token::PLACED; }
{NETS}+						{ return def::DEFParser::token::NETS; }
{NET}+						{ return def::DEFParser::token::NET; }
{PINS}+						{ return def::DEFParser::token::PINS; }


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
