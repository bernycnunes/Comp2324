#include "cdk/ast/expression_node.h"
#include "cdk/ast/typed_node.h"
#include "targets/basic_ast_visitor.h"

namespace til {

class return_node : public cdk::typed_node {
  cdk::expression_node *_value;

public:
  return_node(int lineno, cdk::expression_node *value = nullptr)
      : cdk::typed_node(lineno), _value(value) {}

  cdk::expression_node *value() { return _value; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_return_node(this, level);
  }
};

} // namespace til
