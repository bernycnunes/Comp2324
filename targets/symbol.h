#pragma once

#include <cdk/types/basic_type.h>
#include <memory>
#include <string>

namespace til {

class symbol {
  std::shared_ptr<cdk::basic_type> _type;
  std::string _name;
  int _stack_offest;

public:
  symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name)
      : _type(type), _name(name), _stack_offest(0) {}

  virtual ~symbol() {
    // EMPTY
  }

  static std::shared_ptr<symbol> create(std::string id,
                                        std::shared_ptr<cdk::basic_type> type) {
    return std::make_shared<symbol>(type, id);
  }

  int get_offset() { return _stack_offest; }

  void set_offset(int offset) { _stack_offest = offset; }

  std::shared_ptr<cdk::basic_type> type() const { return _type; }
  bool is_typed(cdk::typename_type name) const { return _type->name() == name; }
  const std::string &name() const { return _name; }
};

} // namespace til
