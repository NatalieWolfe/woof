#include "woof/flags/init_flags.h"

#include <optional>
#include <string_view>
#include <utility>

#include "woof/errors/canonical.h"
#include "woof/errors/macros.h"
#include "woof/flags/cli.h"
#include "woof/flags/flags.h"

namespace woof {
namespace {

/**
 * Parses flags in the form `--name` or `--name=value`. If the value is wrapped
 * in quotes, those quotes will be removed.
 *
 * If the flag begins with "no-" and does not have `=value` suffix, then "no" is
 * used for the value.
 */
std::pair<
  std::optional<std::string_view>,
  std::optional<std::string_view>
> parse_flag(std::string_view flag) {
  // Only long flags are supported, they must start with `--`.
  if (flag.size() < 3 || !flag.starts_with("--")) {
    return {std::nullopt, std::nullopt};
  }
  std::size_t start = 2;
  std::size_t end = start + 1;
  while (end < flag.size() && flag[end] != '=') {
    ++end;
  }
  std::string_view name = flag.substr(start, end - start);
  if (end >= flag.size() - 1) {
    if (name.size() > 3 && name.starts_with("no-")) {
      // If tha name starts with "no-" and has no value, then we will assume it
      // is a boolean flag and "no" is the intended value.
      std::string_view value{name.begin(), 2};
      name.remove_prefix(3);
      return {name, value};
    }
    return {name, std::nullopt};
  }

  start = end + 1;
  end = flag.size();
  if (flag[start] == '"' || flag[start] == '\'') {
    if (flag[start] != flag[end - 1]) {
      throw InvalidArgumentError()
        << "Value for flag " << name << " is malformed.";
    }
    ++start;
    --end;
  }
  return {name, flag.substr(start, end - start)};
}

}

void init_flags(int& argc, const char** argv) {
  WOOF_CHECK_NULL(InvalidArgumentError, argv);

  // The first argument is the program name, so skip that.
  const int original_count = argc;
  const char** mvr = argv + 1;
  const char** back_fill = mvr;
  for (int i = 1; i < original_count; ++i, ++mvr) {
    const int remaining_count = original_count - i - 1;
    auto&& [flag_name, opt_value] = parse_flag(*mvr);
    if (flag_name && flag_exists(*flag_name)) {
      if (flags_cli_set(*flag_name, opt_value, mvr + 1, remaining_count)) {
        // Setting the flag used the next argument, so move past it.
        --argc;
        ++mvr;
        ++i;
      }
      --argc;
    } else {
      // Unknown argument, move it forward in the list.
      *back_fill = *mvr;
      ++back_fill;
    }
  }
}

}
