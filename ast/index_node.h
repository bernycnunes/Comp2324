#pragma once

#include "cdk/ast/lvalue_node.h"

namespace til {

/**
 * Class for describing index nodes.
 */

class index_node : public cdk::lvalue_node {
  cdk::expression_node *_index;
  cdk::expression_node *_base;

public:
  index_node(int lineno, cdk::expression_node *base,
             cdk::expression_node *index)
      : lvalue_node(lineno), _index(index), _base(base) {}

  cdk::expression_node *index() { return _index; }

  cdk::expression_node *base() { return _base; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_index_node(this, level);
  }
};

} // namespace til
