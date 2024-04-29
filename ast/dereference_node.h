#pragma once

#include "cdk/ast/expression_node.h"
#include "cdk/ast/unary_operation_node.h"

namespace til {

/**
 * Class for describing dereference nodes.
 */
class dereference_node : public cdk::unary_operation_node {
public:
  dereference_node(int lineno, cdk::expression_node *argument)
      : cdk::unary_operation_node(lineno, argument) {}

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_dereference_node(this, level);
  }
};
} // namespace til
