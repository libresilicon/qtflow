%define api.prefix {blif}
%error-verbose
%language "c++"
%glr-parser
%debug
%token-table
%define "parser_class_name" {BLIFParser}

%code requires {
	namespace blif {
		class BLIFData;
	}
}

%param {blif::BLIFData *blifdata}

%{
#include <iostream>
#include <string>
#include <QString>
#include "blif/blifscanner.h"
#include "blif/blifdata.h"

#define bliflex (blifdata->getLexer())->bliflex
#define bliflineno (int)(blifdata->getLexer())->lineno()
#define bliftext (blifdata->getLexer())->YYText()

%}

%union {
	int v_int;
	std::string* v_str;
	double v_double;
}

%token VERSION
%token NAMESCASESENSITIVE
%token END
%token DESIGN
%token BUSBITCHARS
%token DISTANCE
%token DIVIDERCHAR
%token UNITS
%token MICRONS
%token BRACKETOPEN
%token BRACKETCLOSE
%token DIEAREA
%token COMPONENTS
%token TRACKS
%token DO
%token LAYER
%token STEP
%token MINUS
%token PLUS
%token PLACED
%token PINS
%token NET
%token NETS
%token ROUTED
%token ASTERISK
%token NEW
%token SPECIALNETS

%token <v_int> INTEGER
%token <v_str> STRING
%token <v_double> DOUBLE

%type <v_double> placement_value

%start blif_file

%%

blif_file: blif_file_options END DESIGN;
blif_file_options: blif_file_option | blif_file_options blif_file_option;
blif_file_option:
	  version
	| cases
	| bitchars
	| dividechar
	| units
	| design
	| diearea
	| tracks
	| components
	| pins
	| nets
	| special_nets
	;

design: DESIGN STRING;
version: VERSION DOUBLE;
cases: NAMESCASESENSITIVE STRING;
bitchars: BUSBITCHARS STRING;
dividechar: DIVIDERCHAR STRING;
units: UNITS DISTANCE MICRONS INTEGER
{
	blifdata->setDistanceUnitMicrons($4);
}
;

diearea:
DIEAREA BRACKETOPEN DOUBLE DOUBLE BRACKETCLOSE BRACKETOPEN DOUBLE DOUBLE BRACKETCLOSE
{
}
|
DIEAREA BRACKETOPEN INTEGER INTEGER BRACKETCLOSE BRACKETOPEN INTEGER INTEGER BRACKETCLOSE
{
	blifdata->setDieArea($3,$4,$7,$8);
}
;

tracks: TRACKS STRING DOUBLE DO INTEGER STEP DOUBLE LAYER STRING | TRACKS STRING DOUBLE DO INTEGER STEP INTEGER LAYER STRING;

components: components_amount component_list END COMPONENTS;

components_amount: COMPONENTS INTEGER
{
	blifdata->setAmountComponents($2);
}
;

component_list:
	  component_list_element
	| component_list_element component_list
	;

component_list_element: component_name component_placement
{
	//blifdata->addUsedModule();
};

component_name: MINUS STRING STRING
{
	blifdata->addUsedModuleNames($2,$3);
};

placement_value:
DOUBLE
{
	$$=$1;
}
|
INTEGER
{
	$$=(double)$1;
};

component_placement: PLUS PLACED BRACKETOPEN placement_value placement_value BRACKETCLOSE STRING
{
	blifdata->addUsedModulePlacement($4,$5);
};

pins: PINS INTEGER pin_list END PINS;
pin_list:
	  pin_list_element
	| pin_list_element pin_list
	;
pin_list_element: MINUS STRING PLUS NET STRING pin_settings;
pin_settings:
	  PLUS pin_setting
	| PLUS pin_setting pin_settings
	;
pin_setting:
	  pin_setting_layer
	| pin_setting_position
	;

pin_setting_layer: LAYER STRING BRACKETOPEN INTEGER INTEGER BRACKETCLOSE BRACKETOPEN INTEGER INTEGER BRACKETCLOSE;
pin_setting_position: PLACED BRACKETOPEN DOUBLE DOUBLE BRACKETCLOSE STRING;

nets: NETS INTEGER net_list END NETS;
net_list:
	  net_list_element
	| net_list_element net_list
;

net_list_element:
	  net_info
	| routed_info
	| new_metal
;

net_name: MINUS STRING;
net_info: net_name net_connections;

net_connections:
	  net_connection
	| net_connection net_connections
	;

net_connection: BRACKETOPEN STRING STRING BRACKETCLOSE;

routed_info:
	  routed_info_layer routed_info_tupels
	| routed_info_layer routed_info_tupels STRING;

routed_info_layer: PLUS ROUTED STRING;

routed_info_tupels:
	  routed_info_tupel
	| routed_info_tupel routed_info_tupels
	;

routed_info_tupel:
	  BRACKETOPEN INTEGER INTEGER BRACKETCLOSE
	| BRACKETOPEN ASTERISK INTEGER BRACKETCLOSE
	| BRACKETOPEN INTEGER ASTERISK BRACKETCLOSE
	| BRACKETOPEN ASTERISK ASTERISK BRACKETCLOSE;

new_metal_tupels:
	  new_metal_tupel
	| new_metal_tupel new_metal_tupels
	;

new_metal_tupel:
	  BRACKETOPEN INTEGER INTEGER BRACKETCLOSE
	| BRACKETOPEN ASTERISK INTEGER BRACKETCLOSE
	| BRACKETOPEN INTEGER ASTERISK BRACKETCLOSE
	| BRACKETOPEN ASTERISK ASTERISK BRACKETCLOSE;

new_metal:
	  NEW STRING new_metal_tupels STRING
	| NEW STRING new_metal_tupels;

special_nets: SPECIALNETS INTEGER special_net_list END SPECIALNETS;

special_net_list:
		special_net_list special_net
	  | special_net;

special_net:
	  special_net_name
	| special_routed_info
	| special_new_metal
;

special_net_name: MINUS STRING;

special_routed_info:
	  special_routed_info_layer special_routed_info_tupels
	| special_routed_info_layer special_routed_info_tupels STRING;

special_routed_info_layer: PLUS ROUTED STRING INTEGER;

special_routed_info_tupels:
	  special_routed_info_tupel
	| special_routed_info_tupel routed_info_tupels
;

special_routed_info_tupel:
	  BRACKETOPEN INTEGER INTEGER BRACKETCLOSE
	| BRACKETOPEN ASTERISK INTEGER BRACKETCLOSE
	| BRACKETOPEN INTEGER ASTERISK BRACKETCLOSE
	| BRACKETOPEN ASTERISK ASTERISK BRACKETCLOSE;

special_new_metal:
	  NEW STRING INTEGER new_metal_tupels STRING
	| NEW STRING INTEGER new_metal_tupels;

%%

void blif::BLIFParser::error(const std::string &s) {
	std::cout << "Error message: " << s << " on line " << bliflineno << ", yytext: " << bliftext <<std::endl;
}
