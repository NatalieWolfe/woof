#pragma once

#include <string>
#include <string_view>

#include "woof/flags/flags.h"
#include "woof/strings/format.h"

WOOF_FLAG(bool, bool_true, true, "A true bool.");
WOOF_FLAG(bool, bool_false, false, "A false bool.");
WOOF_FLAG(
  int, answer_to_life, 42, "The answer to life, the universe, and everything."
);
WOOF_FLAG(float, pi, 3.14f, "Close enough...");
WOOF_FLAG(double, moar_pi, 3.1416, "Closer enough...");
WOOF_FLAG(std::string, str, "Lorem ipsum", "Completely original value.");

WOOF_FLAG(int, change_me, 123, "This value will be changed by tests.");
WOOF_FLAG(bool, change_me_bool, false, "This value will be changed by tests.");
WOOF_FLAG(bool, enable_me, false, "This value will be changed by tests.");
WOOF_FLAG(
  std::string, change_me_str, "", "This value will be changed by tests."
);

// These flags should not compile.
// WOOF_FLAG(const int, const_not_allowed, 42, "Flag types must not be const.");
// WOOF_FLAG(volatile int, vol_not_allowed, 42, "Flags must not be volatile.");
// WOOF_FLAG(int&, ref_not_allowed, 42, "Flag types must not be reference.");
// WOOF_FLAG(
//   int*, ptr_not_allowed, nullptr, "Flag types must not be pointers."
// );
// WOOF_FLAG(
//   std::string_view, no_str_view, "", "Strings must use std::string."
// );

namespace other {

struct CustomType {
  int x = 0;
  int y = 0;
};

}

template <>
struct woof::FlagTraits<other::CustomType> {
  std::string format_method(const other::CustomType& value) const {
    return std::to_string(value.x) + "," + std::to_string(value.y);
  }
  other::CustomType parse_method(std::string_view value) const {
    std::size_t comma_pos = value.find(',');
    return {
      .x = woof::parse<int>({value.data(), comma_pos}),
      .y = woof::parse<int>({
        value.data() + comma_pos + 1,
        value.size() - comma_pos - 1
      })
    };
  }
};

WOOF_FLAG(
  other::CustomType, custom_flag, (other::CustomType{.x = 0, .y = 1}),
  "This is a custom flag type."
);
