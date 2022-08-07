#include "woof/flags/flags.h"

#include <algorithm>
#include <string>
#include <string_view>
#include <unordered_map>

#include "woof/flags/registry.h"

namespace woof {

bool flag_exists(std::string_view flag_name) {
  return get_flag_map().count(hash_flag_name(flag_name)) > 0;
}

}
