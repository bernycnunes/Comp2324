%option c++ prefix="til_scanner_" outfile="til_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{
#include <string>
#include <cstring>
#include <sstream>
#include <cfloat>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "til_parser.tab.h"

// output stream for building string literals
static std::ostringstream strlit;

#define DOUBLE_MAX DBL_MAX   // Maximum value for a 8-byte float

// don't change this
#define yyerror LexerError
%}

%x X_COMMENT
%x X_BACKSLASH
%x X_STRING

DIGIT                            [0-9]
EXPONENT                         [eE][-+]?{DIGIT}+
REAL                             {DIGIT}+{EXPONENT}|({DIGIT}+"."{DIGIT}|{DIGIT}"."{DIGIT}+){EXPONENT}?

%%
  yydebug=1; set_debug(1);

;.*                     ;  /* match the whole line (it is ignored) */

"/*"                    yy_push_state(X_COMMENT);
<X_COMMENT>"*/"         yy_pop_state();
<X_COMMENT>.|"\n"       ;  /* ignore inside comments */

"var"                   return tTYPE_VAR;
"int"                   return tTYPE_INT;
"double"                return tTYPE_DOUBLE;
"string"                return tTYPE_STRING;
"void"                  return tTYPE_VOID;

"set"                   return tSET;

"null"                  return tNULL;
"sizeof"                return tSIZEOF;

"external"              return tEXTERNAL;
"forward"               return tFORWARD;
"public"                return tPUBLIC;

"function"              return tFUNCTION;

">="                    return tGE; 
"<="                    return tLE; 
"=="                    return tEQ; 
"!="                    return tNE;
"&&"                    return tAND;
"||"                    return tOR;
"read"                  return tREAD;

"return"                return tRETURN;
"print"                 return tPRINT;
"println"               return tPRINTLN;

"if"                    return tIF;
"else"                  return tELSE;

"loop"                  return tLOOP; 
"stop"                  return tSTOP;
"next"                  return tNEXT;

"program"               return tPROGRAM;

"block"                 return tBLOCK;

[A-Za-z][A-Za-z0-9]*    { yylval.s = new std::string(yytext); return tIDENTIFIER; }

\"                      yy_push_state(X_STRING);
<X_STRING>\\            yy_push_state(X_BACKSLASH);
<X_STRING>\"            {
                          yylval.s = new std::string(strlit.str());
                          strlit.str("");
                          strlit.clear();
                          yy_pop_state();
                          return tSTRING;
                        }
<X_STRING>.             strlit << yytext;                        

<X_BACKSLASH>"n"        strlit << '\n'; yy_pop_state();
<X_BACKSLASH>"r"        strlit << '\r'; yy_pop_state();
<X_BACKSLASH>"t"        strlit << '\t'; yy_pop_state();
<X_BACKSLASH>"\""       strlit << '\"'; yy_pop_state();
<X_BACKSLASH>[[:xdigit:]]+ {
                          strlit << (char)(unsigned char)strtoul(yytext, NULL, 16);
                          yy_pop_state();
                        }
<X_BACKSLASH>.          strlit << *yytext; yy_pop_state();

0[xX][0-9a-fA-F]+       {
                          long long int value = strtoul(yytext, NULL, 16);
                          if (value > INT_MAX) yyerror("Hexadecimal overflow");
                          yylval.i = value;
                          return tINTEGER;  // Return tINTEGER instead of tHEX
                        }
{REAL}                  {
                          double value = strtod(yytext, nullptr);
                          if (value > DOUBLE_MAX) yyerror("Double overflow");
                          yylval.d = value;
                          return tDOUBLE;
                        }
[0-9]+                  {
                          long long int value = strtol(yytext, nullptr, 10);
                          if (value > INT_MAX) yyerror("Integer overflow");
                          yylval.i = value;
                          return tINTEGER;
                        }

[-()<>=+*/%~;{}@?.\!]   return *yytext;

[ \t\n\r]+              ; /* ignore whitespace */

.                       yyerror("Unknown character");

%%
