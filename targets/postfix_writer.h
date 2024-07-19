#pragma once

#include "targets/basic_ast_visitor.h"

#include <cdk/emitters/basic_postfix_emitter.h>
#include <sstream>
#include <stack>

namespace til {

//!
//! Traverse syntax tree and generate the corresponding assembly code.
//!
class postfix_writer : public basic_ast_visitor {
  cdk::symbol_table<til::symbol> &_symtab;
  cdk::basic_postfix_emitter &_pf;
  int _lbl;
  int _stack_offset;
  bool _global;
  std::vector<std::string> _loop_start_labels;
  std::vector<std::string> _loop_end_labels;

public:
  postfix_writer(std::shared_ptr<cdk::compiler> compiler,
                 cdk::symbol_table<til::symbol> &symtab,
                 cdk::basic_postfix_emitter &pf)
      : basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0),
        _stack_offset(0), _global(true), _loop_start_labels(),
        _loop_end_labels() {}

public:
  ~postfix_writer() { os().flush(); }

private:
  /** Method used to generate sequential labels. */
  inline std::string mklbl(int lbl) {
    std::ostringstream oss;
    if (lbl < 0)
      oss << ".L" << -lbl;
    else
      oss << "_L" << lbl;
    return std::move(oss).str();
  }

public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h" // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end
};

} // namespace til
