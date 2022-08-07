#include <iostream>

#include "woof/flags/flags.h"
#include "woof/flags/init_flags.h"
#include "woof/flags/print_flags.h"
#include "woof/service/server.h"
#include "woof/service/registry.h"

WOOF_FLAG(bool, help, false, "Print this help message and exit.");
WOOF_FLAG(std::string, address, "localhost:54321", "Address to listen on.");

int main(int argc, const char** argv) {
  woof::init_flags(argc, argv);
  if (flags::help) {
    std::cout
      << "Usage: " << argv[0] << " <flags>" << std::endl
      << "Options:" << std::endl;
    woof::print_flags(std::cout);
    return 0;
  }

  woof::Server server;
  woof::attach_services(server);
  server.listen(flags::address);
  server.run();

  return 0;
}
