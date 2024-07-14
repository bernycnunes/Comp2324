#include "cdk/ast/expression_node.h"
#include "cdk/ast/lvalue_node.h"
#include "targets/basic_ast_visitor.h"
namespace til {

class index_node : public cdk::lvalue_node {
  cdk::expression_node *_base;
  cdk::expression_node *_offset;

public:
  index_node(int lineno, cdk::expression_node *base,
             cdk::expression_node *offset)
      : cdk::lvalue_node(lineno), _base(base), _offset(offset) {}

  cdk::expression_node *base() { return _base; }

  cdk::expression_node *offset() { return _offset; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_index_node(this, level);
  }
};

} // namespace til
