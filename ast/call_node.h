#include "cdk/ast/expression_node.h"
#include "cdk/ast/lvalue_node.h"
#include "cdk/ast/sequence_node.h"
#include "targets/basic_ast_visitor.h"

namespace til {

class call_node : public cdk::expression_node {
  cdk::lvalue_node *_callee;
  cdk::sequence_node *_arguments;

public:
  call_node(int lineno, cdk::lvalue_node *c, cdk::sequence_node *args)
      : cdk::expression_node(lineno), _callee(c), _arguments(args) {}

  cdk::lvalue_node *callee() { return _callee; }

  cdk::sequence_node *arguments() { return _arguments; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_call_node(this, level);
  }
};

} // namespace til
