#include "cdk/ast/expression_node.h"
#include "cdk/ast/sequence_node.h"
#include "cdk/types/basic_type.h"
#include "targets/basic_ast_visitor.h"
#include <memory>
namespace til {

class func_def_node : public cdk::expression_node {
  cdk::sequence_node *_arguments;
  til::block_node *_block;
  std::shared_ptr<cdk::basic_type> _return_type;

public:
  func_def_node(int lineno, cdk::sequence_node *args, til::block_node *block,
                std::shared_ptr<cdk::basic_type> rtype)
      : cdk::expression_node(lineno), _arguments(args), _block(block),
        _return_type(rtype) {}

  cdk::sequence_node *arguments() { return _arguments; }

  til::block_node *block() { return _block; }

  std::shared_ptr<cdk::basic_type> return_type() { return _return_type; }

  void accept(basic_ast_visitor *sp, int level) {
    sp->do_func_def_node(this, level);
  }
};

} // namespace til
