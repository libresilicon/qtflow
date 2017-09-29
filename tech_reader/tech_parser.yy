%define api.prefix {tech}
%error-verbose
%language "c++"
%glr-parser
%debug
%token-table
%define "parser_class_name" {TechParser}

%code requires {
	namespace tech {
		class TechData;
	}
}

%param {tech::TechData *techdata}

%{
#include <iostream>
#include <string>
#include <QString>

#include "tech_parser/tech_parser.h"
#include "tech_reader/techdata.h"
#include "tech_reader/techscanner.h"

#define techlex (techdata->getLexer())->techlex
#define techlineno (int)(techdata->getLexer())->lineno()
#define yytext (techdata->getLexer())->YYText()

%}

%union {
	int v_int;
	std::string* v_str;
	double v_double;
}

%token TECH
%token FORMAT
%token END

%token VERSION
%token DESRIPTION

%token ALIASES
%token PLANES
%token TYPES
%token CONTACTS
%token CONTACT
%token STACKABLE

%token STYLES
%token STYLETYPE
%token STYLE

%token COMPOSE
%token PAINT
%token ERASE

%token CONNECT
%token SCALEFACTOR
%token LAYER
%token BLOAT_OR

%token SHRINK
%token GROW
%token LABELS
%token CALMA
%token TEMPLAYER
%token AND_NOT
%token AND
%token OR
%token SQUARES
%token IGNORE
%token MZROUTER
%token NOTACTIVE

%token DRC
%token WIDTH
%token EDGE4WAY
%token SPACING
%token GRIDSPACING
%token AREA
%token EXACT_OVERLAP
%token OVERLAP
%token STEPSIZE
%token LEF
%token ROUTING
%token EXTRACT
%token CSCALE
%token LAMBDA
%token STEP
%token SIDEHALO
%token PLANEORDER
%token RESIST
%token AREACAP
%token PERIMC
%token SIDEOVERLAP
%token SIDEWALL
%token DEVICE
%token FETRESIS

%token WIRING
%token ROUTER

%token PLOWING
%token FIXED
%token COVERED
%token DRAG
%token PLOT
%token OPTIONS
%token SLOTS
%token BBOX
%token GRIDLIMIT
%token NO_OVERLAP
%token QUOTE
%token WIDESPACING
%token VARIANTS
%token VARIANT

%token CIFOUTPUT
%token CIFINPUT
%token CIFSTYLE
%token CIFMAXWIDTH

%token MAXWIDTH
%token SURROUND
%token CUT
%token OBS
%token UNITS
%token SUBSTRATE

%token DEFAULTAREACAP
%token DEFAULTPERIMETER
%token DEFAULTSIDEWALL
%token DEFAULTOVERLAP
%token DEFAULTSIDEOVERLAP


%token <v_int>		INTEGER
%token <v_str>		STRING
%token <v_double>	DOUBLE

%token <std::string> Multiline "multiline-string"

%start tech_file

%%

tech_file:
	tech_sections
;

tech_sections:
	  tech_section
	| tech_sections tech_section
;

tech_section:
	  TECH tech_header END
	| VERSION tech_version END
	| PLANES plane_list END
	| TYPES type_list END
	| CONTACT contact_list stackable_list END
	| ALIASES alias_list END
	| STYLES style_list END
	| COMPOSE compose_list END
	| CONNECT connect_list END
	| CIFOUTPUT cifoutput_list END
	| CIFINPUT cifinput_list END
	| MZROUTER mzrouter_list END
	| DRC drc_list END
	| LEF lef_list END
	| EXTRACT extract_list END
	| WIRING wiring_list END
	| ROUTER router_list END
	| PLOWING plowing_list END
	| PLOT plot_list END
;

tech_header_section:
	  FORMAT INTEGER
	| STRING
;

tech_header:
	  tech_header_section
	| tech_header tech_header_section
;

tech_version_section:
	  Multiline
	| DESRIPTION multiline_comment
;

tech_version:
	  tech_version_section
	| tech_version tech_version_section
;

plane_list:
	  STRING
	| CONTACT
	| plane_list STRING
	| plane_list CONTACT
;

type_list:
	  type_entry
	| type_list type_entry
;

type_entry: STRING STRING | CONTACT STRING;

contact_list:
	  STRING STRING STRING
	| contact_list STRING STRING STRING
;

stackable_list:
		STACKABLE stackable_row
	  | stackable_list STACKABLE stackable_row
;

stackable_row:
	  STRING
	| stackable_row STRING;
;

alias_list:
	  alias_entry
	| alias_list alias_entry
;

alias_entry: STRING STRING;

style_list:
	  style_name style_members
	| style_list style_name style_members
;

style_members:
	  STRING
	| style_members STRING
;

style_name:
	STYLETYPE STRING
;

compose_list:
	  compose_entry
	| compose_list compose_entry
;

compose_entry:
	  COMPOSE STRING STRING STRING
	| PAINT STRING STRING STRING
	| ERASE STRING STRING STRING
;

connect_list:
	  STRING STRING
	| connect_list STRING STRING
;

cifoutput_list:
	  cifoutput_entry
	| cifoutput_list cifoutput_entry
;

cifoutput_entry:
	  STYLE multiline_comment
	| SCALEFACTOR INTEGER INTEGER
	| SCALEFACTOR INTEGER STRING
	| OPTIONS STRING
	| SLOTS INTEGER INTEGER INTEGER INTEGER INTEGER INTEGER
	| BBOX STRING
	| GRIDLIMIT INTEGER
	| LAYER INTEGER STRING
	| LAYER STRING STRING
	| LAYER STRING
	| BLOAT_OR STRING STRING INTEGER
	| BLOAT_OR STRING STRING INTEGER STRING INTEGER
	| SHRINK INTEGER
	| GROW INTEGER
	| LABELS STRING
	| CALMA INTEGER INTEGER
	| TEMPLAYER STRING
	| TEMPLAYER STRING STRING
	| AND_NOT STRING
	| OR STRING
	| AND STRING
	| SQUARES INTEGER INTEGER INTEGER
;

cifinput_list:
	  %empty
	| cifinput_entry
	| cifinput_list cifinput_entry
;

cifinput_entry:
	  STYLE multiline_comment
	| SCALEFACTOR INTEGER
	| SCALEFACTOR INTEGER INTEGER
	| LAYER INTEGER STRING
	| LAYER STRING INTEGER
	| LAYER STRING STRING
	| LAYER STRING
	| BLOAT_OR STRING STRING INTEGER
	| BLOAT_OR STRING STRING INTEGER STRING INTEGER
	| SHRINK INTEGER
	| GROW INTEGER
	| LABELS STRING
	| CALMA INTEGER INTEGER
	| CALMA STRING INTEGER STRING
	| CALMA INTEGER INTEGER STRING
	| TEMPLAYER STRING
	| TEMPLAYER STRING STRING
	| AND_NOT STRING
	| OR STRING
	| AND STRING
	| SQUARES INTEGER INTEGER INTEGER
	| IGNORE STRING
;

mzrouter_list:
	  %empty
	| mzrouter_entry
	| mzrouter_list mzrouter_entry
;

mzrouter_entry:
	  STYLE multiline_comment
	| LAYER STRING INTEGER INTEGER INTEGER INTEGER
	| CONTACT STRING STRING STRING INTEGER
	| NOTACTIVE STRING STRING
;

drc_list:
	  drc_entry
	| drc_list drc_entry
;

drc_entry:
	  multiline_comment
	| STYLE multiline_comment
	| SCALEFACTOR INTEGER
	| WIDTH STRING INTEGER multiline_comment
	| WIDTH STRING STRING multiline_comment
	| EDGE4WAY STRING STRING INTEGER STRING STRING INTEGER multiline_comment
	| EDGE4WAY STRING STRING INTEGER STRING INTEGER INTEGER multiline_comment
	| SPACING STRING STRING INTEGER STRING multiline_comment
	| WIDESPACING STRING INTEGER STRING INTEGER STRING multiline_comment
	| AREA STRING INTEGER INTEGER multiline_comment
	| EXACT_OVERLAP STRING
	| STEPSIZE INTEGER
	| CIFSTYLE STRING
	| CIFSTYLE DRC
	| NO_OVERLAP STRING STRING
	| VARIANTS STRING
	| CIFMAXWIDTH STRING INTEGER STRING multiline_comment
	| MAXWIDTH STRING INTEGER STRING multiline_comment
	| SURROUND STRING STRING INTEGER STRING multiline_comment
;

lef_list:
	  lef_entry
	| lef_list lef_entry
;

lef_entry:
	  CUT STRING STRING STRING STRING STRING
	| OBS STRING STRING
	| IGNORE STRING
	| ROUTING STRING STRING STRING STRING
	| CONTACT STRING STRING STRING STRING
;

extract_list:
	  extract_entry
	| extract_list extract_entry
;

extract_entry:
	  STYLE multiline_comment
	| CSCALE INTEGER
	| LAMBDA INTEGER
	| LAMBDA DOUBLE
	| UNITS STRING
	| STEP INTEGER
	| SIDEHALO INTEGER
	| PLANEORDER STRING INTEGER
	| PLANEORDER CONTACT INTEGER
	| RESIST STRING INTEGER
	| CONTACT STRING INTEGER
	| AREACAP STRING DOUBLE
	| OVERLAP STRING STRING DOUBLE
	| OVERLAP STRING STRING DOUBLE STRING
	| SIDEOVERLAP STRING STRING STRING DOUBLE
	| SIDEOVERLAP STRING STRING STRING DOUBLE STRING
	| PERIMC STRING STRING DOUBLE
	| SIDEWALL STRING STRING STRING STRING DOUBLE
	| FETRESIS STRING STRING INTEGER
	| SUBSTRATE STRING STRING
	| VARIANT STRING
	| DEFAULTAREACAP STRING STRING INTEGER
	| DEFAULTAREACAP STRING STRING STRING INTEGER
	| DEFAULTPERIMETER STRING STRING INTEGER
	| DEFAULTPERIMETER STRING STRING STRING INTEGER
	| DEFAULTSIDEWALL STRING STRING INTEGER
	| DEFAULTOVERLAP STRING STRING STRING STRING INTEGER
	| DEFAULTSIDEOVERLAP STRING STRING STRING STRING INTEGER
	| DEVICE device_params
;

device_params:
	  STRING
	| device_params STRING
;

wiring_list:
	  wiring_entry
	| wiring_list wiring_entry
;

wiring_entry:
	  CONTACT STRING INTEGER STRING INTEGER STRING INTEGER
;

router_list:
	  %empty
	| router_entry
	| router_list router_entry
;

router_entry:
	  GRIDSPACING INTEGER
	| CONTACTS STRING INTEGER
	| STRING STRING INTEGER STRING INTEGER STRING INTEGER;
	| STRING STRING INTEGER STRING INTEGER
;

plowing_list:
	  %empty
	| plowing_entry
	| plowing_list plowing_entry
;

plowing_entry:
	  FIXED STRING
	| COVERED STRING
	| DRAG STRING
;

plot_list:
	  %empty
	| plot_entry
	| plot_list plot_entry
;

plot_entry:
	  STYLE multiline_comment
	| plot_name
	| plot_name plot_multilines
;

plot_multilines:
	  Multiline
	| plot_multilines Multiline
;

plot_name:
	  STRING multiline_comment
	| STRING STRING
	| STRING INTEGER
;

multiline_comment:
	  QUOTE Multiline
	| Multiline
;

%%

void tech::TechParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << techlineno << ", yytext: " << yytext <<std::endl;
}
