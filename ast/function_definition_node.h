#pragma once

#include "cdk/ast/expression_node.h"
#include "cdk/ast/sequence_node.h"
#include "cdk/ast/typed_node.h"

namespace til {

/*
 * Class for describing function nodes.
 */
class function_definition_node : public cdk::expression_node {
  cdk::sequence_node *_arguments;
  til::block_node *_block;
  bool _main;

public:
  function_definition_node(int lineno,
                           std::shared_ptr<cdk::basic_type> outputType,
                           cdk::sequence_node *arguments,
                           til::block_node *block)
      : cdk::expression_node(lineno), _arguments(arguments), _block(block),
        _main(false) {
    std::vector<std::shared_ptr<cdk::basic_type>> inputTypes;
    for (auto *node : arguments->nodes())
      inputTypes.push_back(dynamic_cast<cdk::typed_node *>(node)->type());
    type(cdk::functional_type::create(inputTypes, outputType));
  }

  /**
   * Constructor for the main function.
   */
  function_definition_node(int lineno, til::block_node *block)
      : cdk::expression_node(lineno),
        _arguments(new cdk::sequence_node(lineno)), _block(block), _main(true) {
    type(cdk::functional_type::create(
        cdk::primitive_type::create(4, cdk::TYPE_INT)));
  }

  cdk::sequence_node *arguments() { return _arguments; }

  til::block_node *block() { return _block; }

  bool main() { return _main; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_function_definition_node(this, level);
  }
};

} // namespace til
