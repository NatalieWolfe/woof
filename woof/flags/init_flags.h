#pragma once

namespace woof {

/**
 * Initializes flags from the command-line arguments.
 *
 * Arguments consumed by flags are removed from the argument vector and the
 * argument count is updated appropriately.
 *
 * @param argc  The number of arguments given to the program.
 * @param argv  The vector or command line arguments.
 */
void init_flags(int& argc, const char** argv);

}
