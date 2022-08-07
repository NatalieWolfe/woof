#include "woof/flags/registry.h"

#include <algorithm>
#include <string_view>
#include <string>
#include <unordered_map>

#include "woof/errors/canonical.h"
#include "woof/flags/flag_base.h"

namespace woof {

std::unordered_map<std::size_t, FlagBase*>& get_flag_map() {
  static auto* flags = new std::unordered_map<std::size_t, FlagBase*>{};
  return *flags;
}

std::size_t hash_flag_name(std::string_view flag_name) {
  std::string alt_name;
  alt_name.reserve(flag_name.size());
  for (const char c : flag_name) {
    alt_name.push_back(c == '_' ? '-' : c);
  }
  return std::hash<std::string_view>()(alt_name);
}

void register_flag(FlagBase& flag) {
  const std::size_t flag_hash = hash_flag_name(flag.name());
  if (get_flag_map().count(flag_hash) > 0) {
    throw AlreadyExistsError()
      << "Flag \"" << flag.name() << "\" already exists.";
  }
  get_flag_map().emplace(flag_hash, &flag);
}

}
