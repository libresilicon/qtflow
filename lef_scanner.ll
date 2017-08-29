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

STRING [A-Za-z]|[A-Za-z0-9_,.-<>'"'\[\]\/]

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

%%

{SEMICOLON}.*			{}
{COMMENT}.*				{}
{VERSION}+				{ return lef::LEFParser::token::VERSION; }
{NAMESCASESENSITIVE}+	{ return lef::LEFParser::token::NAMESCASESENSITIVE; }
{BUSBITCHARS}+			{ return lef::LEFParser::token::BUSBITCHARS; }
{DIVIDERCHAR}+			{ return lef::LEFParser::token::DIVIDERCHAR; }
{UNITS}+				{ return lef::LEFParser::token::UNITS; }
{END}+					{ return lef::LEFParser::token::END; }
{DATABASE}+				{ return lef::LEFParser::token::DATABASE; }
{MICRONS}+				{ return lef::LEFParser::token::MICRONS; }

-[0-9]+|[0-9]+ {
	leflval->v_int = atoi(yytext);
	return lef::LEFParser::token::INTEGER;
}

[0-9]+"."[0-9]* {
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
