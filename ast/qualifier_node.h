#pragma once

#include "cdk/ast/basic_node.h"

namespace til {

/**
 * Class for describing qualifier nodes.
 */

enum qualifier_type { EXTERNAL, FORWARD, DEFAULT };

class qualifier_node : public cdk::basic_node {
  qualifier_type _qualifier;

public:
  qualifier_node(int lineno, qualifier_type qualifier)
      : cdk::basic_node(lineno), _qualifier(qualifier) {}

  qualifier_type qualifier() { return _qualifier; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_qualifier_node(this, level);
  }
};

} // namespace til
