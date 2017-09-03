/* C++ parsers require Bison 3 */
%require "3.0"
%language "C++"

/* Write-out tokens header file */
%defines

/* Use Bison's 'variant' to store values. 
 * This allows us to use non POD types (e.g.
 * with constructors/destrictors), which is
 * not possible with the default mode which
 * uses unions.
 */
%define api.value.type variant

/* 
 * Use the 'complete' symbol type (i.e. variant)
 * in the lexer
 */
%define api.token.constructor

/*
 * Add a prefix the make_* functions used to
 * create the symbols
 */
%define api.token.prefix {TOKEN_}

/* Wrap everything in our namespace */
%define api.namespace {vcd}

%define parser_class_name {VCDParser}

/* Extra checks for correct usage */
%define parse.assert

/* Enable debugging info */
%define parse.trace

/* Better error reporting */
%define parse.error verbose

/* 
 * Fixes inaccuracy in verbose error reporting.
 * May be slow for some grammars.
 */
/*%define parse.lac full*/

/* Track locations */
%locations

/* Generate a table of token names */
%token-table

/* The parser expects the lexer to be provided as a constructor argument */
%parse-param {vcd::VCDScanner* lexer}
%parse-param {vcd::VCDLoader* driver}

/* Our yylex implementation expects the lexer to be passed as an argument */
/* %lex-param {vcd::VCDScanner* lexer} */

%{
	namespace vcd {
		class VCDScanner;
		class VCData;
    }

	#include "vcd/vcderror.h"
	#include "vcd/vcdata_types.h"
	#include "vcd/vcdata.h"
	#include "vcd_parser/vcd_parser.h"
	#include "vcd/vcdscanner.h"
	#include "vcd/vcd_loader.hpp"

    //This is not defined by default for some reason...
    #define YY_NULLPTR nullptr

	#define vcdlex (vcdata->getLexer())->vcdlex
	//#define yylex (vcdata->getLexer())->newlex
	#define vcdlineno (int)(vcdata->getLexer())->lineno()
%}

%code top {
#include <iostream> //For cout in error reporting
#include <sstream> //For cout in error reporting
using std::cout;
using std::stringstream;
}

%token DATE "$date"
%token VERSION "$version"
%token COMMENT "$comment"
%token TIMESCALE "$timescale"
%token SCOPE "$scope"
%token MODULE "module"
%token VAR "$var"
%token WIRE "wire"
%token REG "reg"
%token PARAMETER "parameter"
%token UPSCOPE "$upscope"
%token ENDDEFINITIONS "$enddefinitions"
%token DUMPVARS "$dumpvars"
%token END "$end"
%token LOGIC_ONE "1"
%token LOGIC_ZERO "0"
%token LOGIC_UNKOWN "x"
%token LOGIC_HIGHZ "z"
%token <std::string> String "string"
%token <std::string> Multiline "multiline-string"
%token <std::string> VarId "var-id"
%token <std::string> BitString "bit-string"
%token <size_t> Time "time-value"
%token <size_t> Integer "integer-value"
%token <std::string> VarType "var-type"
%token EOF 0 "end-of-file"

%type <Header> vcd_header
%type <std::string> date
%type <std::string> version
%type <std::string> timescale
%type <std::string> scope
%type <Var> var
%type <Var::Type> var_type
%type <LogicValue> LogicValue
%type <std::vector<Var>> definitions


%start vcd_file

%initial-action {
}

%%
vcd_file : vcd_header definitions change_list { 
                driver.vcd_data_ = VcdData(std::move($1), std::move($2), std::move(driver.time_values_));
            }
         ;

vcd_header : date { $$ = Header(); $$.set_date($1); }
           | vcd_header version { $$ = $1; $$.set_version($2); }
           | vcd_header timescale { $$ = $1; $$.set_timescale($2); }
           ;

date : DATE Multiline END { $$ = $2; }
     ;

version : VERSION Multiline END { $$ = $2; }
        ;

timescale : TIMESCALE Multiline END { $$ = $2; }
          ;

scope : SCOPE MODULE String END { $$ = $3; }
      ;

definitions : scope { driver.current_scope_.push_back($1); }
     | definitions scope { $$ = $1; driver.current_scope_.push_back($2); }
     | definitions var { $$ = $1; $$.push_back($2); }
     | definitions upscope { $$ = $1; driver.current_scope_.pop_back(); }
     | definitions enddefinitions { $$ = $1; }
     ;

var : VAR var_type Integer VarId String END { 
            auto hierarchy = driver.current_scope_;
            hierarchy.push_back($5); //Add name to current hierarchy

            $$ = Var($2, $3, driver.generate_var_id($4), $4, hierarchy);
        }
    | VAR var_type Integer VarId String String END { 

            auto hierarchy = driver.current_scope_;

            //Sometimes VCD put the index (e.g. [0]) in a separate string after the base signal name
            //so concatonate them
            hierarchy.push_back($5 + $6);

            $$ = Var($2, $3, driver.generate_var_id($4), $4, hierarchy);
        }
    ;

var_type : WIRE { $$ = Var::Type::WIRE; }
         | REG { $$ = Var::Type::REG; }
         | PARAMETER { $$ = Var::Type::PARAMETER; }
         ;

upscope : UPSCOPE END { }
        ;

enddefinitions : ENDDEFINITIONS END { }

change_list : Time DUMPVARS  { driver.curr_time_ = $1; }
            | change_list Time  { driver.curr_time_ = $2; }
            | change_list LogicValue VarId { 
                    driver.time_values_.push_back(TimeValue(driver.curr_time_, driver.generate_var_id($3), $2));

                    driver.change_count_++;
                    if(driver.change_count_ % 10000000 == 0) {
                        cout << "Loaded " << driver.change_count_ / 1.e6 << "M changes" << std::endl;
                    }
                }
            | change_list END { }
            ;

LogicValue : LOGIC_ONE    { $$ = vcdparse::LogicValue::ONE; }
           | LOGIC_ZERO   { $$ = vcdparse::LogicValue::ZERO; }
           | LOGIC_UNKOWN { $$ = vcdparse::LogicValue::UNKOWN; }
           | LOGIC_HIGHZ  { $$ = vcdparse::LogicValue::HIGHZ; }
           | BitString    { /* ignore for now */ }
           ;

%%

//We need to provide an implementation for parser error handling
void vcd::VCDParser::error(const vcd::location& loc, const std::string& msg) {
	throw vcd::ParseError(msg, loc);
}
