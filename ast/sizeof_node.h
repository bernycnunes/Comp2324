#include "cdk/ast/expression_node.h"
#include "targets/basic_ast_visitor.h"

namespace til {

class sizeof_node : public cdk::expression_node {
  cdk::expression_node *_argument;

public:
  sizeof_node(int lineno, cdk::expression_node *arg)
      : cdk::expression_node(lineno), _argument(arg) {}

  cdk::expression_node *argument() { return _argument; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_sizeof_node(this, level);
  }
};

} // namespace til
