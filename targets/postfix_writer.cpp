#include "targets/postfix_writer.h"
#include ".auto/all_nodes.h" // all_nodes.h is automatically generated
#include "cdk/ast/double_node.h"
#include "cdk/ast/expression_node.h"
#include "cdk/ast/integer_node.h"
#include "cdk/types/reference_type.h"
#include "cdk/types/typename_type.h"
#include "targets/basic_ast_visitor.h"
#include "targets/frame_size_calculator.h"
#include "targets/type_checker.h"
#include <cassert>
#include <iostream>
#include <stdexcept>

//---------------------------------------------------------------------------

void til::postfix_writer::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void til::postfix_writer::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void til::postfix_writer::do_double_node(cdk::double_node *const node,
                                         int lvl) {
  // EMPTY
}
void til::postfix_writer::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, 0);
  _pf.INT(0);
  _pf.EQ();
}
void til::postfix_writer::do_and_node(cdk::and_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  auto lbl = mklbl(++_lbl);
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(lbl);
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(lbl);
}
void til::postfix_writer::do_or_node(cdk::or_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  auto lbl = mklbl(++_lbl);
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(lbl);
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(lbl);
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_sequence_node(cdk::sequence_node *const node,
                                           int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_integer_node(cdk::integer_node *const node,
                                          int lvl) {
  _pf.INT(node->value()); // push an integer
}

void til::postfix_writer::do_string_node(cdk::string_node *const node,
                                         int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;

  /* generate the string */
  _pf.RODATA();                    // strings are DATA readonly
  _pf.ALIGN();                     // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value());      // output string characters

  /* leave the address on the stack */
  _pf.TEXT();            // return to the TEXT segment
  _pf.ADDR(mklbl(lbl1)); // the string to be printed
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_unary_minus_node(cdk::unary_minus_node *const node,
                                              int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG();                           // 2-complement
}

void til::postfix_writer::do_unary_plus_node(cdk::unary_plus_node *const node,
                                             int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_add_node(cdk::add_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void til::postfix_writer::do_sub_node(cdk::sub_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void til::postfix_writer::do_mul_node(cdk::mul_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void til::postfix_writer::do_div_node(cdk::div_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void til::postfix_writer::do_mod_node(cdk::mod_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void til::postfix_writer::do_lt_node(cdk::lt_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void til::postfix_writer::do_le_node(cdk::le_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void til::postfix_writer::do_ge_node(cdk::ge_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void til::postfix_writer::do_gt_node(cdk::gt_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void til::postfix_writer::do_ne_node(cdk::ne_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void til::postfix_writer::do_eq_node(cdk::eq_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_variable_node(cdk::variable_node *const node,
                                           int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  auto s = _symtab.find(node->name());
  if (s->get_offset())
    _pf.LOCAL(s->get_offset());
  else
    _pf.ADDR(node->name());
}

void til::postfix_writer::do_rvalue_node(cdk::rvalue_node *const node,
                                         int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE))
    _pf.LDDOUBLE();
  else
    _pf.LDINT();
}

void til::postfix_writer::do_assignment_node(cdk::assignment_node *const node,
                                             int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP32();

  node->lvalue()->accept(this, lvl); // where to store the value
  _pf.STINT();                       // store the value at address
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_program_node(til::program_node *const node,
                                          int lvl) {
  // Note that til doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.
  ASSERT_SAFE_EXPRESSIONS;
  _global = false;

  auto frame_calc = new til::frame_size_calculator(_compiler, _symtab);
  node->accept(frame_calc, 0);
  int frame_size = frame_calc->localsize();

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(frame_size);

  _symtab.push();
  node->block()->accept(this, lvl);
  _symtab.pop();

  // end the main function
  _pf.INT(0);
  _pf.STFVAL32();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("println");
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_evaluation_node(til::evaluation_node *const node,
                                             int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->is_typed(cdk::TYPE_INT))
    _pf.TRASH(4); // delete the evaluated value
  else if (node->argument()->is_typed(cdk::TYPE_STRING))
    _pf.TRASH(4); // delete the evaluated value's address
  else {
    std::cerr << "ERROR: CANNOT HAPPEN! (postfix :: do_evaluation_node)"
              << std::endl;
    exit(1);
  }
}

void til::postfix_writer::do_print_node(til::print_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (auto a : node->arguments()->nodes()) {
    auto arg = dynamic_cast<cdk::expression_node *>(a);
    if (!arg)
      throw new std::string("print argument is not an expression");

    arg->accept(this, 0); // determine the value to print

    if (arg->is_typed(cdk::TYPE_INT)) {
      _pf.CALL("printi");
      _pf.TRASH(4); // delete the printed value
    } else if (arg->is_typed(cdk::TYPE_DOUBLE)) {
      _pf.CALL("printd");
      _pf.TRASH(8);
    } else if (arg->is_typed(cdk::TYPE_STRING)) {
      _pf.CALL("prints");
      _pf.TRASH(4); // delete the printed value's address
    } else {
      std::cerr << "ERROR: CANNOT HAPPEN! (postfix :: do_print_node)"
                << std::endl;
      exit(1);
    }
  }

  if (node->new_line())
    _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_read_node(til::read_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // TODO:
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_loop_node(til::loop_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  std::string lbl1, lbl2;
  _loop_start_labels.push_back(lbl1 = mklbl(++_lbl));
  _loop_end_labels.push_back(lbl2 = mklbl(++_lbl));
  _pf.LABEL(lbl1);
  node->condition()->accept(this, lvl);
  _pf.JZ(lbl2);
  node->block()->accept(this, lvl + 2);
  _pf.JMP(lbl1);
  _pf.LABEL(lbl2);
  _loop_start_labels.pop_back();
  _loop_end_labels.pop_back();
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_if_node(til::if_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void til::postfix_writer::do_if_else_node(til::if_else_node *const node,
                                          int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

void til::postfix_writer::do_address_of_node(til::address_of_node *const node,
                                             int lvl) {
  // TODO:
}

void til::postfix_writer::do_call_node(til::call_node *const node, int lvl) {
  // TODO:
}

void til::postfix_writer::do_func_def_node(til::func_def_node *const node,
                                           int lvl) {
  // TODO:
}

void til::postfix_writer::do_block_node(til::block_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _symtab.push();
  node->declarations()->accept(this, 0);
  node->instructions()->accept(this, 0);
  _symtab.pop();
}

void til::postfix_writer::do_next_node(til::next_node *const node, int lvl) {
  int depth = node->level();
  std::string jump;
  try {
    jump = _loop_start_labels[_loop_start_labels.size() - depth];
  } catch (std::out_of_range &e) {
    throw std::string("stop level out of bounds");
  }
  _pf.JMP(jump);
}

void til::postfix_writer::do_stop_node(til::stop_node *const node, int lvl) {
  int depth = node->level();
  std::string jump;
  try {
    jump = _loop_end_labels[_loop_end_labels.size() - depth];
  } catch (std::out_of_range &e) {
    throw std::string("stop level out of bounds");
  }
  _pf.JMP(jump);
}

void til::postfix_writer::do_index_node(til::index_node *const node, int lvl) {
  // TODO:
}

void til::postfix_writer::do_declaration_node(til::declaration_node *const node,
                                              int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  auto s = _symtab.find(*node->id());

  if (node->qualifier() == til::qualifier_t::EXTERNAL) {
    _pf.EXTERN(*node->id());
    return;
  }

  if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) ||
      node->is_typed(cdk::TYPE_POINTER)) {
    if (_global) {
      if (node->value())
        _pf.DATA();
      else
        _pf.BSS();

      _pf.ALIGN();
      if (node->qualifier() == til::qualifier_t::PUBLIC)
        _pf.GLOBAL(*node->id(), "OBJ");
      _pf.LABEL(*node->id());

      if (node->value()) {
        auto cast_value = dynamic_cast<cdk::integer_node *>(node->value());
        if (cast_value != nullptr)
          _pf.SINT(cast_value->value());
        else {
          auto cast_value = dynamic_cast<cdk::double_node *>(node->value());
          if (cast_value != nullptr)
            _pf.SDOUBLE(cast_value->value());
          else {
            auto cast_value =
                dynamic_cast<til::address_of_node *>(node->value());
            if (cast_value != nullptr) {
              cast_value->accept(this, 0);
              _pf.STINT();
            }
          }
        }
      } else // no initializer
        _pf.SALLOC(node->type()->size());

    } else { // local variable
      _stack_offset += node->type()->size();
      s->set_offset(-_stack_offset);
      if (node->value()) {
        node->value()->accept(this, 0);
        _pf.LOCAL(-_stack_offset);
        if (node->is_typed(cdk::TYPE_DOUBLE))
          _pf.STDOUBLE();
        else
          _pf.STINT();
      }
    }
  } else if (node->is_typed(cdk::TYPE_STRING)) {
    std::string label;
    if (node->value()) {
      label = mklbl(++_lbl);
      _pf.RODATA();
      _pf.ALIGN();
      _pf.LABEL(label);
      _pf.SSTRING(dynamic_cast<cdk::string_node *>(node->value())->value());
      if (_global) {
        _pf.DATA();
        _pf.ALIGN();
        if (node->qualifier() == til::PUBLIC)
          _pf.GLOBAL(*node->id(), _pf.OBJ());
        _pf.LABEL(*node->id());
        _pf.SADDR(label);
      } else {
        _stack_offset += node->type()->size();
        s->set_offset(_stack_offset);
        if (node->value()) {
          node->value()->accept(this, 0);
          _pf.LOCAL(-_stack_offset);
          _pf.STINT();
        }
      }
    } else {
      if (_global) {
        _pf.BSS();
        _pf.ALIGN();
        if (node->qualifier() == til::PUBLIC)
          _pf.GLOBAL(*node->id(), _pf.OBJ());
        _pf.LABEL(*node->id());
      }
    }
  } else {
    throw std::string("TODO: declaration type not implemented yet");
  }
}

void til::postfix_writer::do_null_node(til::null_node *const node, int lvl) {
  // TODO:
}

void til::postfix_writer::do_objects_node(til::objects_node *const node,
                                          int lvl) {
  node->size()->accept(this, 0);
  auto element_size =
      cdk::reference_type::cast(node->type())->referenced()->size();
  _pf.INT(element_size);
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}

void til::postfix_writer::do_return_node(til::return_node *const node,
                                         int lvl) {
  // TODO:
}

void til::postfix_writer::do_sizeof_node(til::sizeof_node *const node,
                                         int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(node->argument()->type()->size());
}
