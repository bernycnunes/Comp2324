#include "cdk/ast/expression_node.h"
#include "cdk/ast/lvalue_node.h"
#include "targets/basic_ast_visitor.h"

namespace til {

class address_of_node : public cdk::expression_node {
  cdk::lvalue_node *_argument;

public:
  address_of_node(int lineno, cdk::lvalue_node *arg)
      : cdk::expression_node(lineno), _argument(arg) {}

  cdk::lvalue_node *argument() { return _argument; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_address_of_node(this, level);
  }
};

} // namespace til
