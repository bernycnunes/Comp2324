#pragma once

#include "cdk/ast/expression_node.h"
#include "cdk/ast/sequence_node.h"
#include "cdk/ast/typed_node.h"

namespace til {

/*
 * Class for describing function nodes.
 */
class function_node : public cdk::expression_node {
  cdk::typed_node *_type;
  cdk::sequence_node *_arguments;
  cdk::basic_node *_block;

public:
  function_node(int lineno, cdk::typed_node *type,
                cdk::sequence_node *arguments, cdk::basic_node *block)
      : cdk::expression_node(lineno), _type(type), _arguments(arguments),
        _block(block) {}

  cdk::typed_node *type() { return _type; }

  cdk::sequence_node *arguments() { return _arguments; }

  cdk::basic_node *block() { return _block; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_function_node(this, level);
  }
};

} // namespace til
