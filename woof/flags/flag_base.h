#pragma once

#include <string_view>
#include <string>

namespace woof {

class FlagBase {
public:
  explicit FlagBase(
    std::string_view type_name,
    std::string_view name,
    std::string_view description
  );
  FlagBase(const FlagBase&) = delete;
  FlagBase& operator=(const FlagBase&) = delete;

  std::string_view type_name()    const { return _type_name; }
  std::string_view name()         const { return _name; }
  std::string_view description()  const { return _description; }

  virtual std::string default_value_string() const = 0;
  virtual void parse_value(std::string_view value_str) = 0;

private:
  std::string _type_name;
  std::string _name;
  std::string _description;
};

}
