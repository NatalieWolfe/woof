#include "woof/flags/init_flags.h"

#include "gtest/gtest.h"
#include "woof/errors/canonical.h"
#include "woof/flags/flags.h"

WOOF_FLAG(int, flag, 42, "A flag to test with.");
WOOF_FLAG(int, other, 1234, "Another flag to test with.");
WOOF_FLAG(bool, boolean, true, "A flag defaulting to true.");

namespace woof {
namespace {

TEST(InitFlags, ChecksArgsForNull) {
  int argc = 0;
  EXPECT_THROW(init_flags(argc, nullptr), InvalidArgumentError);
}

TEST(InitFlags, SetsKnownFlagArguments) {
  int argc = 2;
  const char* argv[] = {"./binary/name", "--flag=69"};
  flags::flag = 1;
  init_flags(argc, argv);
  EXPECT_EQ(flags::flag, 69);
}

TEST(InitFlags, AllowsSeparatedFlagAndValue) {
  int argc = 3;
  const char* argv[] = {"./binary/name", "--flag", "69"};
  flags::flag = 1;
  init_flags(argc, argv);
  EXPECT_EQ(flags::flag, 69);
}

TEST(InitFlags, RemovesRecognizedFlags) {
  flags::flag = 1;
  flags::other = 2;
  int argc = 5;
  const char* argv[] = {
    "./binary/name", "--flag=69", "--other", "4321", "--this-is-not-used"
  };
  init_flags(argc, argv);
  EXPECT_EQ(argc, 2);

  EXPECT_EQ(flags::flag, 69);
  EXPECT_EQ(flags::other, 4321);

  // Not using testing::ElementsAre becaue `argv` is a 5-element array and we
  // only care about the first values.
  EXPECT_EQ(std::string{argv[0]}, "./binary/name");
  EXPECT_EQ(std::string{argv[1]}, "--this-is-not-used");
}

TEST(InitFlags, ParsesNoPrefixOnArgumentNames) {
  int argc = 2;
  const char* argv[] = {"./binary/name", "--no-boolean"};
  flags::boolean = true;

  init_flags(argc, argv);
  EXPECT_EQ(argc, 1);
  EXPECT_FALSE(flags::boolean);
}

}
}
