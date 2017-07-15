%{
#include <cstdlib>
#include "yyvcd.h"
#include <iostream>

#include <QString>

int vcdlex();
void vcderror(const char *s);
%}

%option header-file="lexvcd.h"
%option outfile="lexvcd.cpp"
%option noyywrap
%option nounput
%option prefix="vcd"

%%

\r\n        { return ENDL;   }
\n          { return ENDL;   }

\$end            { return END;       }
\$date           { return DATE;      }
\$version        { return VERSION;   }
\$timescale      { return TIMESCALE; }
\$scope          { return SCOPE;     }
\$upscope        { return UPSCOPE;   }
\$var            { return VAR;       }
\$enddefinitions { return ENDDEFINITIONS; }
\$dumpvars       { return DUMPVARS; }

[ \t]                    { vcdlval.v_char = yytext[0]; return SPACE;    }
[[:alnum:]]              { vcdlval.v_char = yytext[0]; return ALPHANUM; }
[[:cntrl:][:punct:]]     { vcdlval.v_char = yytext[0]; return SPECIAL;  }

.           { vcderror(yytext); }

%%
