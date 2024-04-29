#pragma once

#include <cdk/ast/expression_node.h>

namespace til {

/**
 * Class for describing loop nodes.
 */
class loop_node : public cdk::basic_node {
  cdk::expression_node *_condition;
  cdk::basic_node *_block;

public:
  inline loop_node(int lineno, cdk::expression_node *condition,
                   cdk::basic_node *block)
      : basic_node(lineno), _condition(condition), _block(block) {}

  inline cdk::expression_node *condition() { return _condition; }
  inline cdk::basic_node *block() { return _block; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_loop_node(this, level);
  }
};

} // namespace til