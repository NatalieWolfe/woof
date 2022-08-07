#pragma once

#include <string_view>
#include <unordered_map>

#include "woof/flags/flag_base.h"

namespace woof {

/**
 * Gets a reference to the underlying flag registry.
 *
 * The registry is a map of hashed flag names to `FlagBase*`.
 */
std::unordered_map<std::size_t, FlagBase*>& get_flag_map();

/**
 * Hashes the string as used for the flag registry.
 */
std::size_t hash_flag_name(std::string_view flag_name);

/**
 * Adds the given flag to the registry.
 */
void register_flag(FlagBase& flag);

}
