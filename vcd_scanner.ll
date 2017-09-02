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

%%

[ \n\t\r]+	{}

END						{ return vcd::VCDParser::token::END;       }
DATE					{ return vcd::VCDParser::token::DATE;      }
VERSION					{ return vcd::VCDParser::token::VERSION;   }
TIMESCALE				{ return vcd::VCDParser::token::TIMESCALE; }
SCOPE					{ return vcd::VCDParser::token::SCOPE;     }
USCOPE					{ return vcd::VCDParser::token::UPSCOPE;   }
VAR						{ return vcd::VCDParser::token::VAR;       }
ENDDEFINITIONS			{ return vcd::VCDParser::token::ENDDEFINITIONS; }
DUMPVARS				{ return vcd::VCDParser::token::DUMPVARS; }

[[:alnum:]]				{ vcdlval->v_char = yytext[0]; return vcd::VCDParser::token::ALPHANUM; }
[[:cntrl:][:punct:]]	{ vcdlval->v_char = yytext[0]; return vcd::VCDParser::token::SPECIAL;  }

%%
