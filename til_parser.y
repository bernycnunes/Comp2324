%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include ".auto/all_nodes.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;          /* integer value */
  double                d;          /* double value */
  std::string          *s;          /* symbol name or string literal */
  cdk::basic_node      *node;       /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  til::block_node      *block;
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tIDENTIFIER tSTRING
%token tIF tLOOP tPRINT tPRINTLN tRETURN tBLOCK tSTOP tNEXT 
%token tEXTERNAL tFORWARD tPUBLIC tVAR
%token tINT_T tDOUBLE_T tSTRING_T tVOID_T
%token tREAD tNULL tSET tINDEX tOBJECTS tSIZEOF tFUNCTION tPROGRAM 

%right '='
%left tGE tLE tEQ tNE '>' '<' tAND tOR
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> inst program decl
%type <sequence> insts args decls
%type <expression> expr
%type <lvalue> lval
%type <block> block

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

program : tPROGRAM block                    { compiler->ast(new til::program_node(LINE, $2)); }
        ;

block : decls insts                         { $$ = new til::block_node(LINE, $1, $2); }
      | decls                               { $$ = new til::block_node(LINE, $1, nullptr); }
      | insts                               { $$ = new til::block_node(LINE, nullptr, $1); }
      ;

args : expr                                 { $$ = new cdk::sequence_node(LINE, $1); }
     | args expr                            { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;

insts : inst                                { $$ = new cdk::sequence_node(LINE, $1); }
      | insts inst                          { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

inst : expr                                 { $$ = new til::evaluation_node(LINE, $1); }
     | '(' tPRINT args ')'                  { $$ = new til::print_node(LINE, $3, false); }
     | '(' tPRINTLN args ')'                { $$ = new til::print_node(LINE, $3, true); }
     | '(' tLOOP '(' expr ')' inst ')'      { $$ = new til::while_node(LINE, $4, $6); }
     | '(' tIF '(' expr ')' inst ')'        { $$ = new til::if_node(LINE, $4, $6); }
     | '(' tIF '(' expr ')' inst inst ')'   { $$ = new til::if_else_node(LINE, $4, $6, $7); }
     ;

decls : decls decl                          { $$ = new cdk::sequence_node(LINE, $2, $1); }
      | decl                                { $$ = new cdk::sequence_node(LINE, $1); }
      ;

decl : '(' tINT_T tIDENTIFIER expr ')'      { $$ = new til::declaration_node(LINE, $3, cdk::primitive_type::create(4, cdk::TYPE_INT), $4); }
     ;

expr : tINTEGER                             { $$ = new cdk::integer_node(LINE, $1); }
     | tDOUBLE                              { $$ = new cdk::double_node(LINE, $1); }
     | tSTRING                              { $$ = new cdk::string_node(LINE, $1); }
     | tNULL                                { $$ = nullptr /* TODO:new til::null_node(LINE, $1)*/; }
     | lval                                 { $$ = new cdk::rvalue_node(LINE, $1); }
     | '-' expr                             { $$ = new cdk::unary_minus_node(LINE, $2); }
     | '+' expr                             { $$ = new cdk::unary_plus_node(LINE, $2); }
     | '(' '+' expr expr ')'                { $$ = new cdk::add_node(LINE, $3, $4); }
     | '(' '-' expr expr ')'                { $$ = new cdk::sub_node(LINE, $3, $4); }
     | '(' '*' expr expr ')'                { $$ = new cdk::mul_node(LINE, $3, $4); }
     | '(' '/' expr expr ')'                { $$ = new cdk::div_node(LINE, $3, $4); }
     | '(' '%' expr expr ')'                { $$ = new cdk::mod_node(LINE, $3, $4); }
     | '(' '<' expr expr ')'                { $$ = new cdk::lt_node(LINE, $3, $4); }
     | '(' '>' expr expr ')'                { $$ = new cdk::gt_node(LINE, $3, $4); }
     | '(' tGE expr expr ')'                { $$ = new cdk::ge_node(LINE, $3, $4); }
     | '(' tLE expr expr ')'                { $$ = new cdk::le_node(LINE, $3, $4); }
     | '(' tNE expr expr ')'                { $$ = new cdk::ne_node(LINE, $3, $4); }
     | '(' tEQ expr expr ')'                { $$ = new cdk::eq_node(LINE, $3, $4); }
     | '(' tAND expr expr ')'               { $$ = new cdk::and_node(LINE, $3, $4); }
     | '(' tOR expr expr ')'                { $$ = new cdk::or_node(LINE, $3, $4); }
     | '(' lval ')'                         { $$ = nullptr /* TODO: new til::call_node(LINE, $2)*/; }
     | '(' tREAD ')'                        { $$ = new til::read_node(LINE); }
     | '(' tSET lval expr ')'               { $$ = new cdk::assignment_node(LINE, $3, $4); }
     ;

lval : tIDENTIFIER                          { $$ = new cdk::variable_node(LINE, $1); }
     ;

%%
