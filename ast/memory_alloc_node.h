#pragma once

#include "cdk/ast/unary_operation_node.h"

namespace til {

/**
 * Class for describing objects nodes.
 */

class memory_alloc_node : public cdk::unary_operation_node {
  cdk::expression_node *_bytes;

public:
  memory_alloc_node(int lineno, cdk::expression_node *argument)
      : cdk::unary_operation_node(lineno, argument) {}

  cdk::expression_node *bytes() { return _bytes; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_memory_alloc_node(this, level);
  }
};
} // namespace til
