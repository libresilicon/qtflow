%{
#include <cstdlib>
#include <iostream>

#include "schematics.yy.h"

#include <QString>

int schematics_lex();
void schematics_error(const char *s);
%}

%option header-file="schematics.ll.h"
%option noyywrap
%option nounput
%option stack
%option prefix="schematics"

LIBS			"LIBS"
COMPONENT		"\$Comp"
END_COMPONENT	"\$EndComp"
%%

LIBS			{ return schematics::SchematicsParser::token::LIBS; }
COMPONENT		{ return schematics::SchematicsParser::token::COMPONENT; }
END_COMPONENT	{ return schematics::SchematicsParser::token::END_COMPONENT; }

%%
