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
  double                d;
  std::string          *s;          /* symbol name or string literal */
  cdk::basic_node      *node;       /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  til::block_node      *block;
  std::vector<std::shared_ptr<cdk::basic_type>> *types;
};

%token <i> tINTEGER
%token <d> tDOUBLE
%token <s> tSTRING tIDENTIFIER
%token <expression> tNULL
%token tAND tOR tGE tLE tEQ tNE '>' '<'
%token tEXTERNAL tFORWARD tPUBLIC tTYPE_VAR tPRIVATE tFUNCTION
%token tTYPE_INT tTYPE_DOUBLE tTYPE_STRING tTYPE_VOID
%token tLOOP tSTOP tNEXT tRETURN
%token tPRINT tPRINTLN tREAD 
%token tPROGRAM tSIZEOF tINDEX tBLOCK tOBJECTS
%token tIF tIFX
%token tSET '+' '-' '*' '/' '%' tUNARY

%type <i> qualifier
%type <node> program instruction
%type <sequence> file declarations exprs instructions
%type <expression> expr initializer fn_def fn_call
%type <lvalue> lval
%type <node> declaration
%type <type> data_type fn_type
%type <types> types
%type <block> block

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file  : program                {compiler->ast($$ = new cdk::sequence_node(LINE, $1)); }
      | declarations           {compiler->ast($$ = $1); }
      | declarations program   {compiler->ast($$ = new cdk::sequence_node(LINE, $2, $1)); }
      |                        {compiler->ast($$ = new cdk::sequence_node(LINE));}
      ;

declarations  : declaration                { $$ = new cdk::sequence_node(LINE, $1);}
              | declarations declaration   { $$ = new cdk::sequence_node(LINE, $2, $1);}
              ;

declaration : '(' data_type tIDENTIFIER initializer ')'             { $$ = new til::declaration_node(LINE, $3, $2, $4); }
            | '(' tTYPE_VAR tIDENTIFIER expr ')'                    { $$ = new til::declaration_node(LINE, $3, $4); }
            | '(' qualifier data_type tIDENTIFIER initializer ')'   { $$ = new til::declaration_node(LINE, $4, $3, $5, (til::qualifier_t) $2); }
            | '(' qualifier tIDENTIFIER expr ')'                    { $$ = new til::declaration_node(LINE, $3, $4, (til::qualifier_t) $2); }
            | '(' qualifier tTYPE_VAR tIDENTIFIER expr ')'          { $$ = new til::declaration_node(LINE, $4, $5, (til::qualifier_t) $2); }
            ;

qualifier : tPUBLIC     { $$ = til::qualifier_t::PUBLIC; }
          | tFORWARD    { $$ = til::qualifier_t::FORWARD; } 
          | tEXTERNAL   { $$ = til::qualifier_t::EXTERNAL; }
          ;

initializer : expr   { $$ = $1; }              
            |        { $$ = nullptr; }
            ;

program : '(' tPROGRAM instructions')'                { $$ = new til::program_node(LINE, new til::block_node(LINE, new cdk::sequence_node(LINE), $3)); }
        | '(' tPROGRAM declarations')'                { $$ = new til::program_node(LINE, new til::block_node(LINE, $3, new cdk::sequence_node(LINE))); }
        | '(' tPROGRAM declarations instructions')'   { $$ = new til::program_node(LINE, new til::block_node(LINE, $3, $4)); }
        ;

instructions  : instruction                   { $$ = new cdk::sequence_node(LINE, $1);}
              | instructions instruction      { $$ = new cdk::sequence_node(LINE, $2, $1);}
              ;

instruction : expr                                            { $$ = $1;}
            | '(' tPRINT exprs')'                             { $$ = new til::print_node(LINE, $3, false); }
            | '(' tPRINTLN exprs')'                           { $$ = new til::print_node(LINE, $3, true); }
            | '(' tSTOP ')'                                   { $$ = new til::stop_node(LINE); }
            | '(' tSTOP tINTEGER ')'                          { $$ = new til::stop_node(LINE,$3); }       
            | '(' tNEXT ')'                                   { $$ = new til::next_node(LINE); }
            | '(' tNEXT tINTEGER ')'                          { $$ = new til::next_node(LINE,$3); }
            | '(' tRETURN ')'                                 { $$ = new til::return_node(LINE, nullptr); }
            | '(' tRETURN expr ')'                            { $$ = new til::return_node(LINE, $3); }
            | '(' tIF expr instruction ')'                    { $$ = new til::if_node(LINE, $3, $4); }
            | '(' tIF expr instruction instruction')'         { $$ = new til::if_else_node(LINE, $3, $4, $5); }
            | '(' tLOOP expr instruction ')'                  { $$ = new til::loop_node(LINE, $3, $4); }
            | '(' tBLOCK block ')'                            { $$ = $3; }
            ;

block : declarations                { $$ = new til::block_node(LINE, $1, new cdk::sequence_node(LINE)); }
      | instructions                { $$ = new til::block_node(LINE, new cdk::sequence_node(LINE), $1); }
      | declarations instructions   { $$ = new til::block_node(LINE, $1, $2); }
      ;

expr  : tINTEGER                       { $$ = new cdk::integer_node(LINE, $1); }
      | tDOUBLE                        { $$ = new cdk::double_node(LINE, $1); }
      | tSTRING                        { $$ = new cdk::string_node(LINE, $1); }
      | tNULL                          { $$ = new til::null_node(LINE); }
      | lval                           { $$ = new cdk::rvalue_node(LINE, $1); }
      | '(' tSET lval expr ')'         { $$ = new cdk::assignment_node(LINE, $3, $4); }
      | '(' '+' expr expr ')'          { $$ = new cdk::add_node(LINE, $3, $4); }
      | '(' '-' expr expr ')'          { $$ = new cdk::sub_node(LINE, $3, $4); }
      | '(' '*' expr expr ')'          { $$ = new cdk::mul_node(LINE, $3, $4); }
      | '(' '/' expr expr ')'          { $$ = new cdk::div_node(LINE, $3, $4); }
      | '(' '%' expr expr ')'          { $$ = new cdk::mod_node(LINE, $3, $4); }
      | '(' '<' expr expr ')'          { $$ = new cdk::lt_node(LINE, $3, $4); }
      | '(' tGE expr expr ')'          { $$ = new cdk::ge_node(LINE, $3, $4); }
      | '(' tLE expr  expr ')'         { $$ = new cdk::le_node(LINE, $3, $4); }
      | '(' tNE expr expr ')'          { $$ = new cdk::ne_node(LINE, $3, $4); }
      | '(' tEQ expr expr ')'          { $$ = new cdk::eq_node(LINE, $3, $4); }
      | '(' tAND expr expr ')'         { $$ = new cdk::and_node(LINE, $3, $4); }
      | '(' tOR expr expr ')'          { $$ = new cdk::or_node (LINE, $3, $4); }
      | '(' '>' expr expr ')'          { $$ = new cdk::gt_node(LINE, $3, $4); }
      | '(' tREAD ')'                  { $$ = new til::read_node(LINE); }
      | '(' '~' expr ')'               { $$ = new cdk::not_node(LINE, $3); }
      | '(' '+' expr ')' %prec tUNARY  { $$ = new cdk::unary_plus_node(LINE, $3); }
      | '(' '-' expr ')' %prec tUNARY  { $$ = new cdk::unary_minus_node(LINE, $3); }
      | '(' '?' lval ')'               { $$ = new til::address_of_node(LINE, $3); }
      | '(' tOBJECTS expr ')'          { $$ = new til::objects_node(LINE, $3); }
      | '(' tSIZEOF expr ')'           { $$ = new til::sizeof_node(LINE, $3); }
      | '(' fn_def ')'                 { $$ = $2; }
      | '(' fn_call ')'                { $$ = $2; }
      ;

fn_def  : tFUNCTION '(' data_type ')'  block                { $$ = new til::func_def_node(LINE, new cdk::sequence_node(LINE), $5, $3); }
        | tFUNCTION '(' data_type declarations')' block     { $$ = new til::func_def_node(LINE, $4, $6, $3); }
        | tFUNCTION '(' data_type ')'                       { $$ = new til::func_def_node(LINE, new cdk::sequence_node(LINE), new til::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)), $3); }
        | tFUNCTION '(' data_type declarations')'           { $$ = new til::func_def_node(LINE, $4, new til::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)), $3); }
        ;

fn_call : expr exprs        { $$ = new til::call_node(LINE, $1, $2); }
        | '@' exprs         { $$ = new til::call_node(LINE, nullptr, $2); }
        | expr              { $$ = new til::call_node(LINE, $1); }
        | '@'               { $$ = new til::call_node(LINE, nullptr, new cdk::sequence_node(LINE)); }



exprs : exprs expr      { $$ = new cdk::sequence_node(LINE, $2, $1); }
      | expr            { $$ = new cdk::sequence_node(LINE, $1); }
      ;

data_type : tTYPE_STRING        { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING); }
          | tTYPE_INT           { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT); }
          | tTYPE_DOUBLE        { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE); }
          | tTYPE_VOID          { $$ = cdk::primitive_type::create(4, cdk::TYPE_VOID);}
          | data_type '!'       { $$ = cdk::reference_type::create(4, $1); }
          | '(' fn_type ')'     { $$ = $2; }
          ;

fn_type : data_type '(' types ')'     { $$ = cdk::functional_type::create(*$3, $1); }
        | data_type                   { $$ = cdk::functional_type::create($1); }   
        ;


types : types data_type     { $$ = $1; $$->push_back($2); }
      | data_type           { $$ = new std::vector<std::shared_ptr<cdk::basic_type>>(); $$->push_back($1); }
      ;

lval  : tIDENTIFIER                 { $$ = new cdk::variable_node(LINE, $1); }
      | '(' tINDEX expr expr ')'    { $$ = new til::index_node(LINE, $3, $4); }
      ;
      
%%
