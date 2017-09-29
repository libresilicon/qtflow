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

%}

%union {
	int v_int;
	std::string* v_str;
	double v_double;
}

%token ASTERISK

%token TECH
%token FORMAT
%token END

%token VERSION
%token DESRIPTION

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
%token CIFOUTPUT
%token CIFINPUT
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
	| CONTACT contact_list END
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
	| DESRIPTION Multiline
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
	  contact_entry
	| contact_list contact_entry
;

contact_entry:
	  STRING STRING STRING
	| STACKABLE STRING STRING STRING
;

style_list:
	  style_entry
	| style_list style_entry
;

style_entry:
	  STYLETYPE STRING
	| STRING INTEGER
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
	  STYLE Multiline
	| SCALEFACTOR INTEGER INTEGER
	| LAYER INTEGER STRING
	| LAYER STRING STRING
	| LAYER STRING
	| BLOAT_OR STRING ASTERISK INTEGER
	| BLOAT_OR STRING ASTERISK INTEGER STRING INTEGER
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
	  cifinput_entry
	| cifinput_list cifinput_entry
;

cifinput_entry:
	  STYLE Multiline
	| SCALEFACTOR INTEGER
	| SCALEFACTOR INTEGER INTEGER
	| LAYER INTEGER STRING
	| LAYER STRING INTEGER
	| LAYER STRING STRING
	| LAYER STRING
	| BLOAT_OR STRING ASTERISK INTEGER
	| BLOAT_OR STRING ASTERISK INTEGER STRING INTEGER
	| SHRINK INTEGER
	| GROW INTEGER
	| LABELS STRING
	| CALMA INTEGER INTEGER
	| CALMA STRING INTEGER ASTERISK
	| CALMA INTEGER INTEGER ASTERISK
	| TEMPLAYER STRING
	| TEMPLAYER STRING STRING
	| AND_NOT STRING
	| OR STRING
	| AND STRING
	| SQUARES INTEGER INTEGER INTEGER
	| IGNORE STRING
;

mzrouter_list:
	  mzrouter_entry
	| mzrouter_list mzrouter_entry
;

mzrouter_entry:
	  STYLE Multiline
	| LAYER STRING INTEGER INTEGER INTEGER INTEGER
	| CONTACT STRING STRING STRING INTEGER
	| NOTACTIVE STRING STRING
;

drc_list:
	  drc_entry
	| drc_list drc_entry
;

drc_entry:
	  Multiline
	| WIDTH STRING INTEGER Multiline
	| WIDTH STRING STRING Multiline
	| EDGE4WAY STRING STRING INTEGER STRING STRING INTEGER Multiline
	| EDGE4WAY STRING STRING INTEGER STRING INTEGER INTEGER Multiline
	| SPACING STRING STRING INTEGER STRING Multiline
	| AREA STRING INTEGER INTEGER Multiline
	| EXACT_OVERLAP STRING
	| STEPSIZE INTEGER
;

lef_list:
	  lef_entry
	| lef_list lef_entry
;

lef_entry:
	  IGNORE STRING
	| ROUTING STRING STRING STRING STRING
	| CONTACT STRING STRING STRING STRING
;

extract_list:
	  extract_entry
	| extract_list extract_entry
;

extract_entry:
	  STYLE Multiline
	| CSCALE INTEGER
	| LAMBDA INTEGER
	| STEP INTEGER
	| SIDEHALO INTEGER
	| PLANEORDER STRING INTEGER
	| PLANEORDER CONTACT INTEGER
	| RESIST STRING INTEGER
	| CONTACT STRING INTEGER INTEGER
	| AREACAP STRING DOUBLE
	| OVERLAP STRING STRING DOUBLE
	| OVERLAP STRING STRING DOUBLE STRING
	| SIDEOVERLAP STRING STRING STRING DOUBLE
	| SIDEOVERLAP STRING STRING STRING DOUBLE STRING
	| PERIMC STRING STRING DOUBLE
	| SIDEWALL STRING STRING STRING STRING DOUBLE
	| DEVICE STRING STRING STRING STRING
	| DEVICE STRING STRING STRING ASTERISK STRING
	| DEVICE STRING STRING STRING STRING STRING STRING INTEGER INTEGER
	| FETRESIS STRING STRING INTEGER
;

wiring_list:
	  wiring_entry
	| wiring_list wiring_entry
;

wiring_entry:
	CONTACT STRING INTEGER STRING INTEGER STRING INTEGER
;

router_list:
	  router_entry
	| router_list router_entry
;

router_entry:
	  GRIDSPACING INTEGER
	| CONTACTS STRING INTEGER
	| STRING STRING INTEGER STRING INTEGER STRING INTEGER;
	| STRING STRING INTEGER STRING INTEGER
;

plowing_list:
	  plowing_entry
	| plowing_list plowing_entry
;

plowing_entry:
	  FIXED STRING
	| COVERED STRING
	| DRAG STRING
;

plot_list:
	  plot_entry
	| plot_list plot_entry
;

plot_entry:
	  STYLE Multiline
	| plot_name
	| plot_name plot_multilines
;

plot_multilines:
	  Multiline
	| plot_multilines Multiline
;

plot_name:
	  STRING Multiline
	| STRING STRING
	| STRING INTEGER
;


%%

void tech::TechParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << techlineno << std::endl;
}
