#include "cdk/ast/expression_node.h"
#include "targets/basic_ast_visitor.h"
namespace til {

class objects_node : public cdk::expression_node {
  cdk::expression_node *_size;

public:
  objects_node(int lineno, cdk::expression_node *size)
      : cdk::expression_node(lineno), _size(size) {}

  cdk::expression_node *size() { return _size; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_objects_node(this, level);
  }
};

} // namespace til
