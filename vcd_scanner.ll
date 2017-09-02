%{
#include <cstdlib>
#include <iostream>
#include <QString>

#include "vcdata.h"

#define YY_DECL int vcd::VCDScanner::vcdlex(vcd::VCDParser::semantic_type* vcdlval, vcd::VCData *vcdata)
%}

%option header-file="vcd.ll.h"
%option prefix="vcd"

%option c++
%option batch
%option stack
%option debug
%option verbose
%option pointer
%option yywrap
%option yylineno
%option nounput

STRING				[a-zA-Z0-9_<>\[\]:.?$/!]*[a-zA-Z_][a-zA-Z0-9_<>\[\]:.?$/!]*
END					"$end"
DATE				"$date"
VERSION				"$version"
TIMESCALE			"$timescale"
SCOPE				"$scope"
USCOPE				"$upscope"
VAR					"$var"
ENDDEFINITIONS		"$enddefinitions"
DUMPVARS			"$dumpvars"
DOLLAR				"$"

%%

[ \n\t\r]+				{ /* skip spaces */ }

END						{ return vcd::VCDParser::token::END;       }
DATE					{ return vcd::VCDParser::token::DATE;      }
VERSION					{ return vcd::VCDParser::token::VERSION;   }
TIMESCALE				{ return vcd::VCDParser::token::TIMESCALE; }
SCOPE					{ return vcd::VCDParser::token::SCOPE;     }
USCOPE					{ return vcd::VCDParser::token::UPSCOPE;   }
VAR						{ return vcd::VCDParser::token::VAR;       }
ENDDEFINITIONS			{ return vcd::VCDParser::token::ENDDEFINITIONS; }
DUMPVARS				{ return vcd::VCDParser::token::DUMPVARS; }
STRING					{ return vcd::VCDParser::token::STRING; }

DOLLAR					{ vcdlval->v_char = yytext[0]; return vcd::VCDParser::token::DOLLAR; }

%%
