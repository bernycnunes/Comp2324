#pragma once

#include "cdk/ast/unary_operation_node.h"

namespace til {

/**
 * Class for describing identity nodes.
 */

class identity_node : public cdk::unary_operation_node {
public:
  identity_node(int lineno, cdk::expression_node *argument)
      : cdk::unary_operation_node(lineno, argument) {}

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_identity_node(this, level);
  }
};
} // namespace til
