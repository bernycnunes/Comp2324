#include "cdk/ast/expression_node.h"
#include "cdk/ast/typed_node.h"
#include "cdk/types/basic_type.h"
#include "cdk/types/primitive_type.h"
#include "cdk/types/typename_type.h"
#include "targets/basic_ast_visitor.h"
#include <cassert>
#include <memory>

namespace til {

typedef enum qualifier_t : int {
  PRIVATE = 0,
  PUBLIC = 1,
  FORWARD = 2,
  EXTERNAL = 3
} qualifier_t;

class declaration_node : public cdk::typed_node {
  std::string *_id;
  til::qualifier_t _qualifier;
  cdk::expression_node *_value;
  std::shared_ptr<cdk::basic_type> _decl_type;

public:
  declaration_node(int lineno, std::string *id,
                   cdk::expression_node *value = nullptr,
                   qualifier_t qualifier = qualifier_t::PRIVATE)
      : cdk::typed_node(lineno), _id(id), _qualifier(qualifier), _value(value),
        _decl_type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)) {}

  declaration_node(int lineno, std::string *id,
                   std::shared_ptr<cdk::basic_type> dtype,
                   cdk::expression_node *value = nullptr,
                   qualifier_t qualifier = qualifier_t::PRIVATE)
      : cdk::typed_node(lineno), _id(id), _qualifier(qualifier), _value(value),
        _decl_type(dtype) {}

  std::string *id() { return _id; }

  std::shared_ptr<cdk::basic_type> decl_type() { return _decl_type; }

  til::qualifier_t qualifier() { return _qualifier; }

  cdk::expression_node *value() { return _value; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_declaration_node(this, level);
  }
};

} // namespace til
