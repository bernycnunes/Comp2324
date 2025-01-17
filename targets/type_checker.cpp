#include "targets/type_checker.h"
#include ".auto/all_nodes.h" // automatically generated
#include "cdk/ast/expression_node.h"
#include "cdk/types/basic_type.h"
#include "cdk/types/reference_type.h"
#include "cdk/types/typename_type.h"
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"
#include <cassert>
#include <cdk/types/functional_type.h>
#include <cdk/types/primitive_type.h>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#define ASSERT_UNSPEC                                                          \
  {                                                                            \
    if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC))          \
      return;                                                                  \
  }

//---------------------------------------------------------------------------

void til::type_checker::do_sequence_node(cdk::sequence_node *const node,
                                         int lvl) {
  for (auto elm : node->nodes())
    elm->accept(this, 0);
}

//---------------------------------------------------------------------------

void til::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}

void til::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}

void til::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}

void til::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, 0);
  if (!node->argument()->is_typed(cdk::TYPE_INT))
    throw std::string("Invalid argument type");
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, 0);
  if (!node->left()->is_typed(cdk::TYPE_INT))
    throw std::string("Invalid left type");

  node->right()->accept(this, 0);
  if (!node->right()->is_typed(cdk::TYPE_INT))
    throw std::string("Invalid right type");

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, 0);
  if (!node->left()->is_typed(cdk::TYPE_INT))
    throw std::string("Invalid left type");

  node->right()->accept(this, 0);
  if (!node->right()->is_typed(cdk::TYPE_INT))
    throw std::string("Invalid right type");

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void til::type_checker::do_integer_node(cdk::integer_node *const node,
                                        int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void til::type_checker::processUnaryExpression(
    cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, 0);
  if (!node->argument()->is_typed(cdk::TYPE_INT))
    throw std::string("wrong type in argument of unary expression");

  // in til, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_unary_minus_node(cdk::unary_minus_node *const node,
                                            int lvl) {
  processUnaryExpression(node, lvl);
}

void til::type_checker::do_unary_plus_node(cdk::unary_plus_node *const node,
                                           int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void til::type_checker::processBinaryExpression(
    cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, 0);
  if (!node->left()->is_typed(cdk::TYPE_INT))
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, 0);
  if (!node->right()->is_typed(cdk::TYPE_INT))
    throw std::string("wrong type in right argument of binary expression");

  // in til, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void til::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void til::type_checker::do_variable_node(cdk::variable_node *const node,
                                         int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<til::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr)
    node->type(symbol->type());
  else
    throw id;
}

void til::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void til::type_checker::do_assignment_node(cdk::assignment_node *const node,
                                           int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, 0);
  node->type(node->lvalue()->type());
  node->rvalue()->accept(this, 0);
  if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC))
    node->rvalue()->type(node->type());
  else if (node->type()->to_string() != node->rvalue()->type()->to_string())
    throw std::string("type mismatch");
}

//---------------------------------------------------------------------------

void til::type_checker::do_program_node(til::program_node *const node,
                                        int lvl) {
  auto _prev_rtype = _curr_rtype;
  _curr_rtype = cdk::primitive_type::create(4, cdk::TYPE_INT);
  node->block()->accept(this, 0);
  _curr_rtype = _prev_rtype;
}

void til::type_checker::do_evaluation_node(til::evaluation_node *const node,
                                           int lvl) {
  node->argument()->accept(this, 0);
}

void til::type_checker::do_print_node(til::print_node *const node, int lvl) {
  node->arguments()->accept(this, 0);
  for (size_t i = 0; i < node->arguments()->nodes().size(); i++) {
    auto arg = node->arguments()->node(i);
    if (dynamic_cast<cdk::expression_node *>(arg) == nullptr)
      throw std::string("Invalid print argument type on arg #") +
          std::to_string(i);
  }
}

//---------------------------------------------------------------------------

void til::type_checker::do_read_node(til::read_node *const node, int lvl) {
  // TODO:
}

//---------------------------------------------------------------------------

void til::type_checker::do_loop_node(til::loop_node *const node, int lvl) {
  node->condition()->accept(this, 0);
  if (!(node->condition()->is_typed(cdk::TYPE_INT) ||
        node->condition()->is_typed(cdk::TYPE_DOUBLE)))
    throw std::string("Condition is of a wrong type");
}

//---------------------------------------------------------------------------

void til::type_checker::do_if_node(til::if_node *const node, int lvl) {
  node->condition()->accept(this, 0);
  if (!(node->condition()->is_typed(cdk::TYPE_INT) ||
        node->condition()->is_typed(cdk::TYPE_DOUBLE)))
    throw std::string("Condition is of a wrong type");
}

void til::type_checker::do_if_else_node(til::if_else_node *const node,
                                        int lvl) {
  node->condition()->accept(this, 0);
  if (!(node->condition()->is_typed(cdk::TYPE_INT) ||
        node->condition()->is_typed(cdk::TYPE_DOUBLE)))
    throw std::string("Condition is of a wrong type");
}

void til::type_checker::do_address_of_node(til::address_of_node *const node,
                                           int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, 0);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void til::type_checker::do_call_node(til::call_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->callee()->accept(this, 0);
  if (!node->callee()->is_typed(cdk::TYPE_FUNCTIONAL))
    throw std::string("Callee is not of functional type");
  auto callee_type = cdk::functional_type::cast(node->callee()->type());

  node->arguments()->accept(this, 0);

  if (node->arguments()->size() != callee_type->input_length())
    throw std::string("Incompatible number of function call arguments");

  for (size_t i = 0; i < node->arguments()->size(); ++i) {
    auto arg = dynamic_cast<cdk::expression_node *>(node->arguments()->node(i));
    auto t1 = arg->type();
    auto t2 = callee_type->input()->component(i);
    if (t1->to_string() != t2->to_string())
      throw std::to_string(i) + std::string("th argument is the wrong type");
  }

  node->type(callee_type->output(0));
}

void til::type_checker::do_func_def_node(til::func_def_node *const node,
                                         int lvl) {
  ASSERT_UNSPEC;

  node->arguments()->accept(this, 0);

  std::vector<std::shared_ptr<cdk::basic_type>> arg_types;
  for (auto arg : node->arguments()->nodes())
    arg_types.push_back(dynamic_cast<til::declaration_node *>(arg)->type());

  node->type(cdk::functional_type::create(arg_types, node->return_type()));

  auto _prev_rtype = _curr_rtype;
  _curr_rtype = node->return_type();
  node->block()->accept(this, 0);
  _curr_rtype = _prev_rtype;
}

void til::type_checker::do_block_node(til::block_node *const node, int lvl) {
  _symtab.push();
  node->declarations()->accept(this, 0);
  node->instructions()->accept(this, 0);
  _symtab.pop();
}

void til::type_checker::do_next_node(til::next_node *const node, int lvl) {
  // NOTE: no need cus `(next <int literal>)` is checked by parser
}

void til::type_checker::do_stop_node(til::stop_node *const node, int lvl) {
  // NOTE: no need cus `(stop <int literal>)` is checked by parser
}

void til::type_checker::do_index_node(til::index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->base()->accept(this, 0);
  if (!node->base()->is_typed(cdk::TYPE_POINTER))
    throw std::string("Base expected to be of type pointer");

  node->offset()->accept(this, 0);
  if (!node->offset()->is_typed(cdk::TYPE_INT))
    throw std::string("Offset expected to be of type integer");
}

void til::type_checker::do_declaration_node(til::declaration_node *const node,
                                            int lvl) {
  if (_symtab.find_local(*node->id()))
    throw std::string("Variable ") + *node->id() + "Already delcared";

  if (node->type() == nullptr || node->is_typed(cdk ::TYPE_UNSPEC)) {
    if (node->value()) {
      node->value()->accept(this, 0);
      if (node->decl_type()->name() != cdk::TYPE_UNSPEC &&
          node->decl_type()->to_string() != node->value()->type()->to_string())
        throw std::string("Declared type and initializer type mismatch");
      node->type(node->value()->type());
    } else if (node->decl_type()->name() == cdk::TYPE_UNSPEC)
      throw std::string(
          "Declaration without explicit type requires an initializer");
    else
      node->type(node->decl_type());
  }

  auto s = til::symbol::create(*node->id(), node->type());
  _symtab.insert(*node->id(), s);
}

void til::type_checker::do_null_node(til::null_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::reference_type::create(
      4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)));
}

void til::type_checker::do_objects_node(til::objects_node *const node,
                                        int lvl) {
  if (node->type() == nullptr)
    node->type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
  if (node->is_typed(cdk ::TYPE_UNSPEC))
    return;

  node->size()->accept(this, 0);
  if (!node->size()->is_typed(cdk::TYPE_INT))
    throw std::string("Expected argument of type integer");

  auto prev_type = node->type();
  node->type(cdk::reference_type::create(4, prev_type));
}

void til::type_checker::do_return_node(til::return_node *const node, int lvl) {
  ASSERT_UNSPEC;

  if (node->value()) {
    node->value()->accept(this, 0);
    node->type(node->value()->type());
  } else
    node->type(cdk::primitive_type::create(0, cdk::TYPE_VOID));

  if (!node->is_typed(_curr_rtype->name()))
    throw std::string("Function and return types mismatch");
}

void til::type_checker::do_sizeof_node(til::sizeof_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, 0);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}
