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
%option nounput

STRING	[a-zA-Z0-9_<>\[\]:.?$/!]*[a-zA-Z_][a-zA-Z0-9_<>\[\]:.?$/!]*
END		\$end
%%

\r\n					{}
\n						{}

END						{ return vcd::VCDParser::token::END;       }
\$date					{ return vcd::VCDParser::token::DATE;      }
\$version				{ return vcd::VCDParser::token::VERSION;   }
\$timescale				{ return vcd::VCDParser::token::TIMESCALE; }
\$scope					{ return vcd::VCDParser::token::SCOPE;     }
\$upscope				{ return vcd::VCDParser::token::UPSCOPE;   }
\$var					{ return vcd::VCDParser::token::VAR;       }
\$enddefinitions		{ return vcd::VCDParser::token::ENDDEFINITIONS; }
\$dumpvars				{ return vcd::VCDParser::token::DUMPVARS; }

[ \t]					{ vcdlval->v_char = yytext[0]; return vcd::VCDParser::token::SPACE;    }
[[:alnum:]]				{ vcdlval->v_char = yytext[0]; return vcd::VCDParser::token::ALPHANUM; }
[[:cntrl:][:punct:]]	{ vcdlval->v_char = yytext[0]; return vcd::VCDParser::token::SPECIAL;  }

%%
