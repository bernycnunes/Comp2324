#pragma once

#include <cdk/types/basic_type.h>
#include <memory>
#include <string>

namespace til {

class symbol {
  std::shared_ptr<cdk::basic_type> _type;
  std::string _name;

public:
  symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name)
      : _type(type), _name(name) {}

  virtual ~symbol() {
    // EMPTY
  }

  std::shared_ptr<cdk::basic_type> type() const { return _type; }
  bool is_typed(cdk::typename_type name) const { return _type->name() == name; }
  const std::string &name() const { return _name; }
};

} // namespace til
