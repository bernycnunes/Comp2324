#pragma once

#include "cdk/ast/basic_node.h"
#include "targets/basic_ast_visitor.h"

namespace til {

/**
 * Class for describing recursion nodes.
 */

class recursion_node : public cdk::basic_node {
public:
  recursion_node(int lineno) : cdk::basic_node(lineno) {}

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_recursion_node(this, level);
  }
};
} // namespace til
