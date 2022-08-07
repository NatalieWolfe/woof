#pragma once

#include <ostream>

namespace woof {

/**
 * Formats all of the flags with their type, defaults, and description suitable
 * for printing to a terminal window.
 *
 * @param out
 *  An output stream to write the formatted flag descriptions.
 */
void print_flags(std::ostream& out);

}
