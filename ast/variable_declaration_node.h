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
  qualifier_node *_qualifier;
  std::string *_identifier;
  cdk::rvalue_node *_rvalue;

public:
  variable_declaration_node(int lineno, qualifier_node *qualifier,
                            std::string *identifier,
                            cdk::rvalue_node *rvalue = nullptr,
                            std::shared_ptr<cdk::basic_type> type = nullptr)
      : cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier),
        _rvalue(rvalue) {
    this->type(type ? type : cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
  }

  qualifier_node *qualifier() { return _qualifier; }

  std::string *identifier() { return _identifier; }

  cdk::rvalue_node *rvalue() { return _rvalue; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_variable_declaration_node(this, level);
  }
};

} // namespace til
