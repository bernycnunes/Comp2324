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
  til::block_node      *block;      /* block node */
};

%token <i> tINTEGER tDOUBLE tNULL
%token tAND tOR tSIZEOF 
%token tPRINT tPRINTLN tPUBLIC tPRIVATE
%token <s> tIDENTIFIER tSTRING
%token tLOOP tIF tREAD tPROGRAM tVAR tBLOCK
%token tTYPE_INT tTYPE_STRING tTYPE_VOID tTYPE_VAR tTYPE_DOUBLE
%token tEXTERNAL tFORWARD tRETURN 
%token tSTOP tNEXT
%token tHEX tSET
%token tFUNCTION

%nonassoc tIFX
%nonassoc tELSE

%right '='
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY
%left '(' ')'

%type <node> stmt decl program decl_function
%type <sequence> top_level list decls exprs
%type <expression> expr opt_initializer
%type <lvalue> lval
%type <type> type
%type <i> opt_integer 
%type <block> block // Change the type of block

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

start : top_level { compiler->ast($1); }
      ;

top_level : top_level decl { $$ = new cdk::sequence_node(LINE, $2, dynamic_cast<cdk::sequence_node*>($1)); }
          | top_level program { $$ = new cdk::sequence_node(LINE, $2, dynamic_cast<cdk::sequence_node*>($1)); }
          | decl { $$ = new cdk::sequence_node(LINE, $1); }
          | program { $$ = new cdk::sequence_node(LINE, $1); }
          ;

program : '(' tPROGRAM list ')' { $$ = new til::program_node(LINE, $3); }
        ;

list : stmt { $$ = new cdk::sequence_node(LINE, $1); }
     | list stmt { $$ = new cdk::sequence_node(LINE, $2, $1); }
     | decl { $$ = new cdk::sequence_node(LINE, $1); }
     | list decl { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;

stmt : expr ';' { $$ = new til::evaluation_node(LINE, $1); }
     | tREAD lval ';' { $$ = new til::read_node(LINE, $2); }
     | '(' tLOOP expr stmt ')' { $$ = new til::loop_node(LINE, $3, $4); }
     | '(' tIF expr stmt ')' %prec tIFX { $$ = new til::if_node(LINE, $3, $4); }
     | '(' tIF expr stmt stmt ')' { $$ = new til::if_else_node(LINE, $3, $4, $5); }
     | '(' tPRINTLN exprs ')' { $$ = new til::print_node(LINE, $3); }
     | '(' tRETURN expr ')' { $$ = new til::return_node(LINE, $3); }
     | '(' tSET lval expr ')' { $$ = new cdk::assignment_node(LINE, $3, $4); }
     | block { $$ = $1; }
     | '(' tNEXT ')' { $$ = new til::next_node(LINE); }
     | '(' tSTOP opt_integer ')' { $$ = new til::stop_node(LINE, $3); }
     ;

opt_integer : /* empty */ { $$ = 0; }
            | tINTEGER { $$ = $1; }
            ;

block : '(' tBLOCK decls list ')' { $$ = new til::block_node(LINE, $3, $4); }
      ;

decl : '(' type tIDENTIFIER opt_initializer ')' { $$ = new til::variable_declaration_node(LINE, tPRIVATE, $2, *$3, $4); }
     | tPRIVATE '(' type tIDENTIFIER opt_initializer ')' { $$ = new til::variable_declaration_node(LINE, tPRIVATE, $3, *$4, $5); }
     | decl_function // Add this rule to include function declarations
     ;

decl_function : '(' tFUNCTION type tIDENTIFIER '(' decls ')' block ')' { $$ = new til::function_definition_node(LINE, $3, $6, $8); } // Add this rule for function declarations
             ;

opt_initializer : /* empty */ { $$ = nullptr; }
                | expr { $$ = $1; }
                ;

type : tTYPE_INT { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT); }
     | tTYPE_STRING { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING); }
     | tTYPE_VOID { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID); }
     | tTYPE_VAR { $$ = cdk::primitive_type::create(4, cdk::TYPE_UNSPEC); }
     | tTYPE_DOUBLE { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE); }
     ;

exprs : expr %prec tIFX { $$ = new cdk::sequence_node(LINE, $1); }
      | exprs expr { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

expr : tINTEGER { $$ = new cdk::integer_node(LINE, $1); }
     | tDOUBLE { $$ = new cdk::double_node(LINE, $1); }
     | tSTRING { $$ = new cdk::string_node(LINE, $1); }
     | tNULL { $$ = new til::null_node(LINE); }
     | '-' expr %prec tUNARY { $$ = new cdk::unary_minus_node(LINE, $2); }
     | '+' expr %prec tUNARY { $$ = new cdk::unary_plus_node(LINE, $2); }
     | '~' expr %prec tUNARY { $$ = new cdk::not_node(LINE, $2); }
     | '+' expr expr { $$ = new cdk::add_node(LINE, $2, $3); }
     | '-' expr expr { $$ = new cdk::sub_node(LINE, $2, $3); }
     | '*' expr expr { $$ = new cdk::mul_node(LINE, $2, $3); }
     | '/' expr expr { $$ = new cdk::div_node(LINE, $2, $3); }
     | '%' expr expr { $$ = new cdk::mod_node(LINE, $2, $3); }
     | '<' expr expr { $$ = new cdk::lt_node(LINE, $2, $3); }
     | '>' expr expr { $$ = new cdk::gt_node(LINE, $2, $3); }
     | tGE expr expr { $$ = new cdk::ge_node(LINE, $2, $3); }
     | tLE expr expr { $$ = new cdk::le_node(LINE, $2, $3); }
     | tNE expr expr { $$ = new cdk::ne_node(LINE, $2, $3); }
     | tEQ expr expr { $$ = new cdk::eq_node(LINE, $2, $3); }
     | tAND expr expr { $$ = new cdk::and_node(LINE, $2, $3); }
     | tOR expr expr { $$ = new cdk::or_node(LINE, $2, $3); }
     | '(' expr ')' { $$ = $2; }
     | lval { $$ = new cdk::rvalue_node(LINE, $1); }
     | lval '=' expr { $$ = new cdk::assignment_node(LINE, $1, $3); }
     | tSIZEOF expr { $$ = new til::sizeof_node(LINE, $2); }
     ;

lval : tIDENTIFIER { $$ = new cdk::variable_node(LINE, $1); }
     ;

decls : /* empty */ { $$ = new cdk::sequence_node(LINE); }
      | decls decl { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

%%
