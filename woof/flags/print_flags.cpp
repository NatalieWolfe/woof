#include "woof/flags/print_flags.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <list>
#include <ostream>
#include <string>

#include "woof/flags/flag_base.h"
#include "woof/flags/registry.h"

namespace woof {
namespace {

constexpr std::size_t FLAG_PRINT_WIDTH  = 80;
constexpr std::size_t MAX_FLAG_NAME     = 20;
constexpr std::string_view FLAG_PREFIX  = " --";

}

void print_flags(std::ostream& out) {
  std::list<const FlagBase*> flags;
  std::size_t name_pad = 0;
  for (const auto& [hash, flag] : get_flag_map()) {
    flags.push_back(flag);
    name_pad = std::max(name_pad, flag->name().size());
  }
  flags.sort([](const FlagBase* a, const FlagBase* b) {
    return std::lexicographical_compare(
      a->name().begin(), a->name().end(),
      b->name().begin(), b->name().end()
    );
  });

  name_pad = std::min(name_pad + 1, MAX_FLAG_NAME);
  const auto& padding = std::string(name_pad + FLAG_PREFIX.size(), ' ');
  for (const FlagBase* flag : flags) {
    if (flag->name().size() >= MAX_FLAG_NAME) {
      out << FLAG_PREFIX << flag->name() << std::endl << padding;
    } else {
      out << FLAG_PREFIX << std::setw(name_pad) << std::left << flag->name();
    }

    // TODO(alaina): Wrap the description at FLAG_PRINT_WIDTH. Bonus points to
    // use the terminal's actual width as the wrap point.
    out
      << flag->type_name() << "; Default = " << flag->default_value_string()
      << std::endl
      << padding << flag->description() << std::endl << std::endl;
  }
}

}
