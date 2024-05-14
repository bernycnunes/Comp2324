#pragma once

#include "cdk/ast/basic_node.h"
#include "cdk/ast/rvalue_node.h"
#include "cdk/ast/typed_node.h"
#include "cdk/types/primitive_type.h"

namespace til {

/**
 * Class for describing declaration nodes.
 */
class variable_declaration_node : public cdk::typed_node {
  int _qualifier;
  std::string _identifier;
  cdk::expression_node *_init;

public:
  variable_declaration_node(int lineno, int qualifier,
                            std::shared_ptr<cdk::basic_type> type,
                            const std::string &identifier,
                            cdk::expression_node *init)
      : cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier),
        _init(init) {
    cdk::typed_node::type(type);
  }

  int qualifier() { return _qualifier; }

  std::string identifier() { return _identifier; }

  cdk::expression_node *init() { return _init; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_variable_declaration_node(this, level);
  }
};

} // namespace til
