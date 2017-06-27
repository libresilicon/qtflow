%{
#include <cstdlib>
#include "yymagic.h"

#include <QString>

int yylex();
void yyerror(const char *s);
%}

%option header-file="lexmagic.h"
%option outfile="lexmagic.cpp"
%option noyywrap
%option nounput

%%

#.*         { /* comment */ }

magic       { return MAGIC; }
tech        { return TECH; }
magscale    { return MAGSCALE; }
timestamp   { return TIMESTAMP; }

rect        { return RECT; }
rlabel      { return RLABEL; }

\<\<\           { return BEGINTITLE; }
\ \>\>          { return ENDTITLE; }
\<\<\ end\ \>\> { return END; }

[[:alpha:]][[:alnum:]<>]+      { yylval.v_str = yytext; return IDENT; }
[0-9-]+      { yylval.v_int = atoi(yytext); return INTEGER; }

[ \t]       { return SPACE; }
\r\n        { return ENDL;  }
\n          { return ENDL;  }

.           { yyerror("invalid character"); }

%%
