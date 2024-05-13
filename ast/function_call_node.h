#pragma once

#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace til {

/**
 * Class for describing function call nodes.
 */
class function_call_node : public cdk::expression_node {
  cdk::expression_node *_function;
  cdk::sequence_node *_arguments;

public:
  /**
   * Constructor for a function call without arguments.
   * An empty sequence is automatically inserted to represent
   * the missing arguments.
   */
  function_call_node(int lineno, cdk::expression_node *function)
      : cdk::expression_node(lineno), _function(function),
        _arguments(new cdk::sequence_node(lineno)) {}

  /**
   * Constructor for a function call with arguments.
   */
  function_call_node(int lineno, cdk::expression_node *function,
                     cdk::sequence_node *arguments)
      : cdk::expression_node(lineno), _function(function),
        _arguments(arguments) {}

  cdk::expression_node *function() { return _function; }

  cdk::sequence_node *arguments() { return _arguments; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_function_call_node(this, level);
  }
};

} // namespace til