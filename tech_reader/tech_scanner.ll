%{
#include <cstdlib>
#include <QString>

#include "tech_parser/tech_parser.h"
#include "tech_reader/techscanner.h"

#define YY_DECL int tech::TechScanner::techlex(\
	tech::TechParser::semantic_type* techlval,\
	tech::TechData *techdata)
%}

%option prefix="tech"
%option c++
%option batch
%option stack
%option debug
%option verbose
%option pointer
%option yywrap
%option nounput
%option yylineno

TILDE			"~"
STRING			[A-Za-z]|[~A-Za-z0-9_,.-<>\[\]\/\(\)]
QUOTES			"\""
COMMENT			"#"
ASTERISK		"*"

TECH			"tech"
END				"end"
FORMAT			"format"

VERSION			"version"
DESRIPTION		"description"

PLANES			"planes"
TYPES			"types"
CONTACT			"contact"

STACKABLE		"stackable"
STYLES			"styles"
STYLETYPE		"styletype"

COMPOSE			"compose"
PAINT			"paint"
ERASE			"erase"
CONNECT			"connect"
CIFOUTPUT		"cifoutput"
STYLE			"style"
SCALEFACTOR		"scalefactor"
LAYER			"layer"
BLOAT_OR		"bloat-or"
GROW			"grow"
SHRINK			"shrink"
LABELS			"labels"
CALMA			"calma"
TEMPLAYER		"templayer"
AND_NOT			"and-not"
OR				"or"
SQUARES			"squares"
AND				"and"
CIFINPUT		"cifinput"
IGNORE			"ignore"
MZROUTER		"mzrouter"
NOTACTIVE		"notactive"
DRC				"drc"
WIDTH			"width"
EDGE4WAY		"edge4way"
SPACING			"spacing"
AREA			"area"
EXACT_OVERLAP	"exact_overlap"
STEPSIZE		"stepsize"

%x multiline

%%

{QUOTES}+ {
	BEGIN(multiline);
	return tech::TechParser::token::QUOTES;
}

{COMMENT}.*			{}

{TECH}+				{ return tech::TechParser::token::TECH; }
{FORMAT}+			{ return tech::TechParser::token::FORMAT; }
{END}+				{ return tech::TechParser::token::END; }

{VERSION}+ {
	BEGIN(multiline);
	return tech::TechParser::token::VERSION;
}

{DESRIPTION}+ {
	BEGIN(multiline);
	return tech::TechParser::token::DESRIPTION;
}

{PLANES}+			{ return tech::TechParser::token::PLANES; }
{TYPES}+			{ return tech::TechParser::token::TYPES; }
{CONTACT}+			{ return tech::TechParser::token::CONTACT; }

{STACKABLE}+		{ return tech::TechParser::token::STACKABLE; }

{STYLES}+			{ return tech::TechParser::token::STYLES; }
{STYLETYPE}+		{ return tech::TechParser::token::STYLETYPE; }

{COMPOSE}+			{ return tech::TechParser::token::COMPOSE; }
{PAINT}+			{ return tech::TechParser::token::PAINT; }
{ERASE}+			{ return tech::TechParser::token::ERASE; }
{CONNECT}+			{ return tech::TechParser::token::CONNECT; }
{CIFOUTPUT}+		{ return tech::TechParser::token::CIFOUTPUT; }
{STYLE}+ {
	BEGIN(multiline);
	return tech::TechParser::token::STYLE;
}
{SCALEFACTOR}+		{ return tech::TechParser::token::SCALEFACTOR; }
{LAYER}+			{ return tech::TechParser::token::LAYER; }
{BLOAT_OR}+			{ return tech::TechParser::token::BLOAT_OR; }
{ASTERISK}+			{ return tech::TechParser::token::ASTERISK; }
{GROW}+				{ return tech::TechParser::token::GROW; }
{SHRINK}+			{ return tech::TechParser::token::SHRINK; }
{LABELS}+			{ return tech::TechParser::token::LABELS; }
{CALMA}+			{ return tech::TechParser::token::CALMA; }
{TEMPLAYER}+		{ return tech::TechParser::token::TEMPLAYER; }
{AND_NOT}+			{ return tech::TechParser::token::AND_NOT; }
{OR}+				{ return tech::TechParser::token::OR; }
{SQUARES}+			{ return tech::TechParser::token::SQUARES; }
{AND}+				{ return tech::TechParser::token::AND; }
{CIFINPUT}+			{ return tech::TechParser::token::CIFINPUT; }
{IGNORE}+			{ return tech::TechParser::token::IGNORE; }
{MZROUTER}+			{ return tech::TechParser::token::MZROUTER; }
{NOTACTIVE}+		{ return tech::TechParser::token::NOTACTIVE; }
{DRC}+				{ return tech::TechParser::token::DRC; }
{WIDTH}+			{ return tech::TechParser::token::WIDTH; }
{EDGE4WAY}+			{ return tech::TechParser::token::EDGE4WAY; }
{SPACING}+			{ return tech::TechParser::token::SPACING; }
{AREA}+				{ return tech::TechParser::token::AREA; }
{EXACT_OVERLAP}+	{ return tech::TechParser::token::EXACT_OVERLAP; }
{STEPSIZE}+			{ return tech::TechParser::token::STEPSIZE; }

<multiline>.* {
	std::cout << "Multiline: " << YYText() << "\n";
	BEGIN(INITIAL);
	return tech::TechParser::token::Multiline;
}

-[0-9]+|[0-9]+ {
	techlval->v_int = atoi(yytext);
	return tech::TechParser::token::INTEGER;
}

[0-9]+"."[0-9]* {
	techlval->v_double = atof(yytext);
	return tech::TechParser::token::DOUBLE;
}

{STRING}* {
	techlval->v_str = new std::string(yytext, yyleng);
	return tech::TechParser::token::STRING;
}

[ \n\t\r\\]+ {
	/* yylloc->step(); */
}

%%
