#include "targets/xml_writer.h"
#include ".auto/all_nodes.h" // automatically generated
#include "ast/loop_node.h"
#include "targets/basic_ast_visitor.h"
#include "targets/type_checker.h"
#include <iostream>
#include <ostream>
#include <string>

//---------------------------------------------------------------------------

void til::xml_writer::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void til::xml_writer::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void til::xml_writer::do_double_node(cdk::double_node *const node, int lvl) {
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<value>" << node->value() << "</value>"
       << std::endl;
  closeTag(node, lvl);
}
void til::xml_writer::do_not_node(cdk::not_node *const node, int lvl) {
  do_unary_operation(node, lvl);
}

void til::xml_writer::do_and_node(cdk::and_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}

void til::xml_writer::do_or_node(cdk::or_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_sequence_node(cdk::sequence_node *const node,
                                       int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size()
       << "'>" << std::endl;
  for (auto elm : node->nodes()) {
    elm->accept(this, lvl + 2);
  }

  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_integer_node(cdk::integer_node *const node, int lvl) {
  process_literal(node, lvl);
}

void til::xml_writer::do_string_node(cdk::string_node *const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_unary_operation(cdk::unary_operation_node *const node,
                                         int lvl) {
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void til::xml_writer::do_unary_minus_node(cdk::unary_minus_node *const node,
                                          int lvl) {
  do_unary_operation(node, lvl);
}

void til::xml_writer::do_unary_plus_node(cdk::unary_plus_node *const node,
                                         int lvl) {
  do_unary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_binary_operation(
    cdk::binary_operation_node *const node, int lvl) {
  openTag(node, lvl);

  openTag("left_operand", lvl + 2);
  node->left()->accept(this, lvl + 4);
  closeTag("left_operand", lvl + 2);

  openTag("right_operand", lvl + 2);
  node->right()->accept(this, lvl + 4);
  closeTag("right_operand", lvl + 2);

  closeTag(node, lvl);
}

void til::xml_writer::do_add_node(cdk::add_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_sub_node(cdk::sub_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_mul_node(cdk::mul_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_div_node(cdk::div_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_mod_node(cdk::mod_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_lt_node(cdk::lt_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_le_node(cdk::le_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_ge_node(cdk::ge_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_gt_node(cdk::gt_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_ne_node(cdk::ne_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}
void til::xml_writer::do_eq_node(cdk::eq_node *const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_variable_node(cdk::variable_node *const node,
                                       int lvl) {
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name()
       << "</" << node->label() << ">" << std::endl;
}

void til::xml_writer::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void til::xml_writer::do_assignment_node(cdk::assignment_node *const node,
                                         int lvl) {
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl);
  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_program_node(til::program_node *const node, int lvl) {
  openTag(node, lvl);
  node->block()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_evaluation_node(til::evaluation_node *const node,
                                         int lvl) {
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void til::xml_writer::do_print_node(til::print_node *const node, int lvl) {
  openTag(node, lvl);
  node->arguments()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_read_node(til::read_node *const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_loop_node(til::loop_node *const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void til::xml_writer::do_if_node(til::if_node *const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void til::xml_writer::do_if_else_node(til::if_else_node *const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

void til::xml_writer::do_address_of_node(til::address_of_node *const node,
                                         int lvl) {
  openTag(node, lvl);

  openTag("argument", lvl + 2);
  node->argument()->accept(this, lvl + 4);
  closeTag("argument", lvl + 2);

  closeTag(node, lvl);
}

void til::xml_writer::do_call_node(til::call_node *const node, int lvl) {
  openTag(node, lvl);

  openTag("callee", lvl + 2);
  node->callee()->accept(this, lvl + 4);
  closeTag("callee", lvl + 2);

  if (node->arguments()) {
    openTag("arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("arguments", lvl + 2);
  }

  closeTag(node, lvl);
}

void til::xml_writer::do_func_def_node(til::func_def_node *const node,
                                       int lvl) {
  openTag(node, lvl);

  openTag("arguments", lvl + 2);
  node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);

  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);

  closeTag(node, lvl);
}

void til::xml_writer::do_block_node(til::block_node *const node, int lvl) {
  openTag(node, lvl);

  openTag("declarations", lvl + 2);
  node->declarations()->accept(this, lvl + 4);
  closeTag("declarations", lvl + 2);

  openTag("instructions", lvl + 2);
  node->instructions()->accept(this, lvl + 4);
  closeTag("instructions", lvl + 2);

  closeTag(node, lvl);
}

void til::xml_writer::do_next_node(til::next_node *const node, int lvl) {
  openTag(node, lvl);

  os() << std::string(lvl + 2, ' ') << "<level>" << node->level() << "</level>"
       << std::endl;

  closeTag(node, lvl);
}

void til::xml_writer::do_stop_node(til::stop_node *const node, int lvl) {
  openTag(node, lvl);

  os() << std::string(lvl + 2, ' ') << "<level>" << node->level() << "</level>"
       << std::endl;

  closeTag(node, lvl);
}

void til::xml_writer::do_index_node(til::index_node *const node, int lvl) {
  openTag(node, lvl);

  openTag("base", lvl + 2);
  node->base()->accept(this, lvl + 4);
  closeTag("base", lvl + 2);

  openTag("offset", lvl + 2);
  node->offset()->accept(this, lvl + 4);
  closeTag("offset", lvl + 2);

  closeTag(node, lvl);
}

void til::xml_writer::do_declaration_node(til::declaration_node *const node,
                                          int lvl) {
  openTag(node, lvl);

  os() << std::string(lvl + 2, ' ') << "<id>" << *node->id() << "</id>"
       << std::endl;

  os() << std::string(lvl + 2, ' ') << "<qualifier>" << node->qualifier()
       << "</qualifier>" << std::endl;

  if (node->value()) {
    openTag("value", lvl + 2);
    node->value()->accept(this, lvl + 4);
    closeTag("value", lvl + 2);
  }

  closeTag(node, lvl);
}

void til::xml_writer::do_null_node(til::null_node *const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

void til::xml_writer::do_objects_node(til::objects_node *const node, int lvl) {
  openTag(node, lvl);

  os() << std::string(lvl + 2, ' ') << "<size>" << node->size() << "</size>"
       << std::endl;

  closeTag(node, lvl);
}

void til::xml_writer::do_return_node(til::return_node *const node, int lvl) {
  openTag(node, lvl);

  if (node->value()) {
    openTag("value", lvl + 2);
    node->value()->accept(this, lvl + 4);
    closeTag("value", lvl + 2);
  }

  closeTag(node, lvl);
}

void til::xml_writer::do_sizeof_node(til::sizeof_node *const node, int lvl) {
  openTag(node, lvl);

  openTag("argument", lvl + 2);
  node->argument()->accept(this, lvl + 4);
  closeTag("argument", lvl + 2);

  closeTag(node, lvl);
}
