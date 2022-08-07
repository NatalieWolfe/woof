#pragma once
/**
 * @file
 * Define a new command-line flag. The value will be stored as a
 * `woof::Flag<type>` in the `flags` namespace.
 *
 * Example:
 * ```cpp
 * WOOF_FLAG(int, max_foo, 1, "Maximum foo to bar.");
 *
 * int main() {
 *   const int foo_limit = flags::max_foo;
 *   std::cout << foo_limit << std::endl;
 * }
 * ```
 */

#include <cstdlib>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>

#include "woof/flags/flag_base.h"
#include "woof/strings/format.h"

#define WOOF_FLAG(type, name, default_value, description)               \
  namespace flags {                                                     \
    ::woof::Flag<type> name{#type, #name, default_value, description};  \
  }                                                                     \

#define WOOF_DECLARE_FLAG(type, name) \
  namespace flags { extern ::woof::Flag<type> name; }

namespace woof {

/**
 * Returns true if a flag with the name given has been defined by the
 * application. Any underscores in the name are converted to hyphens before
 * checking.
 */
bool flag_exists(std::string_view flag_name);

// -------------------------------------------------------------------------- //

/**
 * @brief Type traits that assist in working with arbitrary flag types.
 */
template <typename T>
struct FlagTraits {
  std::string format_method(const T& value) const { return format(value); }
  T parse_method(std::string_view value) const { return parse<T>(value); }
};

// -------------------------------------------------------------------------- //

template <typename T>
class Flag: public FlagBase {
public:
  static_assert(
    !std::is_same_v<T, char*> && !std::is_same_v<T, const char*> &&
    !std::is_same_v<T, std::string_view>,
    "String flags must use std::string."
  );
  static_assert(
    std::is_same_v<std::remove_reference_t<std::remove_cv_t<T>>, T>,
    "Flags must use non-const, non-volatile, non-reference types."
  );
  static_assert(
    std::is_same_v<std::remove_pointer_t<T>, T>,
    "Flags types cannot be pointers."
  );

  explicit Flag(
    std::string_view type_name,
    std::string_view name,
    T default_value,
    std::string_view description
  ):
    FlagBase{type_name, name, description},
    _default{std::move(default_value)}
  {}

  const T& value() const {
    return _value ? *_value : _default;
  }

  operator const T&() const {
    return value();
  }

  template <typename U>
  bool operator==(U&& other) const {
    return value() == other;
  }

  template <typename U>
  bool operator!=(U&& other) const {
    return value() != other;
  }

  std::string default_value_string() const override {
    return FlagTraits<T>{}.format_method(_default);
  }

  void parse_value(std::string_view value_str) override {
    set_value(FlagTraits<T>{}.parse_method(value_str));
  }

  template<typename U>
  void set_value(U&& value) {
    _value = std::forward<U>(value);
  }

  template<typename U>
  Flag& operator=(U&& value) {
    set_value(std::forward<U>(value));
    return *this;
  }

private:
  const T _default;
  std::optional<T> _value;
};

}
