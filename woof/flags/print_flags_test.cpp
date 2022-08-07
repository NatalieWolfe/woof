#include "woof/flags/print_flags.h"

#include <sstream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "woof/flags/test_flags.h"

namespace woof {
namespace {

using ::testing::MatchesRegex;

TEST(PrintFlags, PrintsFlagDescriptions) {
  std::stringstream out;
  print_flags(out);

  EXPECT_THAT(
    out.str(),
    MatchesRegex(".*The answer to life, the universe, and everything.*")
  );
}

TEST(PrintFlags, PrintsInAlphabeticalOrder) {
  std::stringstream out;
  print_flags(out);

  EXPECT_THAT(
    out.str(),
    MatchesRegex(
      ".*answer_to_life"
      ".*bool_false"
      ".*bool_true"
      ".*change_me"
      ".*change_me_bool"
      ".*change_me_str"
      ".*moar_pi"
      ".*pi.*"
    )
  );
}

}
}
