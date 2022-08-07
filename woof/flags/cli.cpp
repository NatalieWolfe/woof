#include "woof/flags/cli.h"

#include <optional>
#include <string_view>
#include <string>

#include "woof/errors/canonical.h"
#include "woof/flags/flag_base.h"
#include "woof/flags/registry.h"

namespace woof {
namespace {

constexpr std::string_view BOOLEAN_TYPE_NAME = "bool";

}

bool flags_cli_set(
  std::string_view flag_name,
  std::optional<std::string_view> value,
  const char* const* rest_args,
  int argc
) {
  auto flag_itr = get_flag_map().find(hash_flag_name(flag_name));

  // If there wasn't a value set and the flag wasn't found, we may have a
  // boolean flag being disabled with a `no` prefix or by changing `enable` to
  // `disable` in the flag name.
  std::string alternate_flag_name;
  if (flag_itr == get_flag_map().end() && !value) {
    if (flag_name.size() > 2 && flag_name.starts_with("no")) {
      flag_name.remove_prefix(2);
      if (
        flag_name.size() > 1 &&
        (flag_name[0] == '-' || flag_name[0] == '_')
      ) {
        flag_name.remove_prefix(1);
      }
    } else if (flag_name.size() > 7 && flag_name.starts_with("disable")) {
      flag_name.remove_prefix(1);   // -d
      alternate_flag_name = std::string{flag_name};
      alternate_flag_name[0] = 'e'; // -i
      alternate_flag_name[1] = 'n'; // -s
      flag_name = alternate_flag_name;
    }
    value = "no";
    flag_itr = get_flag_map().find(hash_flag_name(flag_name));
  }

  // Did we find the flag?
  if (flag_itr == get_flag_map().end()) {
    throw NotFoundError() << "Flag " << flag_name << " not found.";
  }

  // Set the value of the flag.
  FlagBase* flag = flag_itr->second;
  if (!value && flag->type_name() == BOOLEAN_TYPE_NAME) value = "yes";
  if (value) {
    flag->parse_value(*value);
    return false;
  } else if (argc > 0) {
    std::string_view next_arg{*rest_args};
    if (!next_arg.starts_with("--")) {
      flag->parse_value(next_arg);
      return true;
    }
  }
  throw InvalidArgumentError()
    << "Flag " << flag_name << " missing " << flag->type_name() << " value.";
}

}
