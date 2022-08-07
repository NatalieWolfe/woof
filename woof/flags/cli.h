#pragma once

#include <optional>
#include <string_view>

namespace woof {

/**
 * Sets the value of the named flag using either the optional value or the next
 * argument.
 *
 * @param flag_name
 *  The name of the flag, what appears between the `--` and `=`.
 * @param value
 *  Any text value parsed after the first `=` if there is one.
 * @param rest_args
 *  A pointer to the next argument after the one being set.
 * @param argc
 *  The number of arguments after this one.
 *
 * @return
 *  True if the next argument in `rest_args` was used as the value for this
 *  argument. Otherwise, false is returned.
 */
bool flags_cli_set(
  std::string_view flag_name,
  std::optional<std::string_view> value,
  const char* const* rest_args,
  int argc
);

}
