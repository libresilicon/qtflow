%{
#include <cstdlib>
#include "yysettings.h"

#include <QString>

int settingslex();
void settingserror(const char *s);
%}

%option header-file="lexsettings.h"
%option outfile="lexsettings.cpp"
%option noyywrap
%option nounput
%option prefix="settings"

%%

#.*         { /* comment */ }

=           { return ASSIGN; }
set         { return SET; }
[ \t]*      { return SPACES; }
\r\n        { return ENDL;  }
\n          { return ENDL;  }

[[:alnum:]<>/]+      { settingslval.v_str = yytext; return STRING; }


.           { settingserror("invalid character"); }

%%
