#pragma once

#include "cdk/ast/sequence_node.h"

namespace til {

/**
 * Class for describing print nodes.
 */
class print_node : public cdk::basic_node {
  cdk::sequence_node *_arguments;
  bool _new_line;

public:
  print_node(int lineno, cdk::sequence_node *arguments, bool nl)
      : cdk::basic_node(lineno), _arguments(arguments), _new_line(nl) {}

  cdk::sequence_node *arguments() { return _arguments; }

  bool new_line() { return _new_line; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_print_node(this, level);
  }
};

} // namespace til
