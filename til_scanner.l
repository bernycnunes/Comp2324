%option c++ prefix="til_scanner_" outfile="til_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "til_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
%x X_STRING
%%
  yydebug=1; set_debug(1);

(?:;.*|\/\*.*\*\/)          ; /* ignore comments */

">="                        return tGE; 
"<="                        return tLE; 
"=="                        return tEQ; 
"!="                        return tNE; 
"&&"                        return tAND;
"||"                        return tOR;

"int"                       return tTYPE_INT;
"double"                    return tTYPE_DOUBLE;
"string"                    return tTYPE_STRING;
"void"                      return tTYPE_VOID;

"external"                  return tEXTERNAL;
"forward"                   return tFORWARD;
"public"                    return tPUBLIC;
"var"                       return tTYPE_VAR;

"if"                        return tIF; 
"loop"                      return tLOOP; 
"print"                     return tPRINT; 
"println"                   return tPRINTLN; 
"return"                    return tRETURN; 
"block"                     return tBLOCK;
"stop"                      return tSTOP;
"next"                      return tNEXT;

"read"                      return tREAD; 
"null"                      return tNULL;
"set"                       return tSET;
"index"                     return tINDEX;
"objects"                   return tOBJECTS;
"sizeof"                    return tSIZEOF;
"function"                  return tFUNCTION;

"program"                   return tPROGRAM; 

([A-Za-z][A-Za-z0-9_]*|@)   yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                          yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"                yy_pop_state(); return tSTRING;
<X_STRING>\\\"              *yylval.s += yytext + 1;
<X_STRING>\\n               *yylval.s += '\n';
<X_STRING>\\t               *yylval.s += '\t';
<X_STRING>\\r               *yylval.s += '\r';
<X_STRING>.                 *yylval.s += yytext;
<X_STRING>\n                yyerror("newline in string");
<X_STRING>\\[0-9]{1,3}      *yylval.s += (char) std::stoi(std::string(yytext + 1), nullptr, 8);

[0-9]+                      yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;
0x[0-9A-Fa-f]+              yylval.i = strtol(yytext, nullptr, 16); return tINTEGER;
[0-9]+\.[0-9]+(E[0-9]+)?    yylval.d = strtod(yytext, nullptr); return tDOUBLE;

[-()<>=+*/%;{}.?!~@]        return *yytext;

[ \t\n]+                    ; /* ignore whitespace */

.                           yyerror("Unknown character");

%%
