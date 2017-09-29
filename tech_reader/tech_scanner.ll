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
STRING			[A-Za-z]|[~A-Za-z0-9_,.-<>\[\]\/\(\)$]
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
CONTACTS		"contacts"
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
GRIDSPACING		"gridspacing"
AREA			"area"
EXACT_OVERLAP	"exact_overlap"
OVERLAP			"overlap"
STEPSIZE		"stepsize"
LEF				"lef"
ROUTING			"routing"
EXTRACT			"extract"
CSCALE			"cscale"
LAMBDA			"lambda"
STEP			"step"
SIDEHALO		"sidehalo"
PLANEORDER		"planeorder"
RESIST			"resist"
AREACAP			"areacap"
PERIMC			"perimc"
SIDEOVERLAP		"sideoverlap"
SIDEWALL		"sidewall"
DEVICE			"device"
FETRESIS		"fetresis"
WIRING			"wiring"
ROUTER			"router"
PLOWING			"plowing"
FIXED			"fixed"
COVERED			"covered"
DRAG			"drag"
PLOT			"plot"

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
{CONTACTS}+			{ return tech::TechParser::token::CONTACTS; }
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
{GRIDSPACING}+		{ return tech::TechParser::token::GRIDSPACING; }
{AREA}+				{ return tech::TechParser::token::AREA; }
{EXACT_OVERLAP}+	{ return tech::TechParser::token::EXACT_OVERLAP; }
{OVERLAP}+			{ return tech::TechParser::token::OVERLAP; }
{STEPSIZE}+			{ return tech::TechParser::token::STEPSIZE; }
{LEF}+				{ return tech::TechParser::token::LEF; }
{ROUTING}+			{ return tech::TechParser::token::ROUTING; }
{EXTRACT}+			{ return tech::TechParser::token::EXTRACT; }
{CSCALE}+			{ return tech::TechParser::token::CSCALE; }
{LAMBDA}+			{ return tech::TechParser::token::LAMBDA; }
{STEP}+				{ return tech::TechParser::token::STEP; }
{SIDEHALO}+			{ return tech::TechParser::token::SIDEHALO; }
{PLANEORDER}+		{ return tech::TechParser::token::PLANEORDER; }
{RESIST}+			{ return tech::TechParser::token::RESIST; }
{AREACAP}+			{ return tech::TechParser::token::AREACAP; }
{PERIMC}+			{ return tech::TechParser::token::PERIMC; }
{SIDEOVERLAP}+		{ return tech::TechParser::token::SIDEOVERLAP; }
{SIDEWALL}+			{ return tech::TechParser::token::SIDEWALL; }
{DEVICE}+			{ return tech::TechParser::token::DEVICE; }
{FETRESIS}+			{ return tech::TechParser::token::FETRESIS; }
{WIRING}+			{ return tech::TechParser::token::WIRING; }
{ROUTER}+			{ return tech::TechParser::token::ROUTER; }
{PLOWING}+			{ return tech::TechParser::token::PLOWING; }
{FIXED}+			{ return tech::TechParser::token::FIXED; }
{COVERED}+			{ return tech::TechParser::token::COVERED; }
{DRAG}+				{ return tech::TechParser::token::DRAG; }
{PLOT}+				{ return tech::TechParser::token::PLOT; }

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
