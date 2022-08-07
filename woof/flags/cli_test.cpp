#include "woof/flags/cli.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "woof/errors/canonical.h"
#include "woof/flags/test_flags.h"

namespace woof {
namespace {

TEST(FlagsCliSet, ChangesFlagsByName) {
  flags::change_me = 1234;
  flags_cli_set("change_me", "4321", nullptr, 0);
  EXPECT_EQ(flags::change_me, 4321);
}

TEST(FlagsCliSet, ConvertsUnderscoresToDashes) {
  flags::change_me = 1234;
  flags_cli_set("change-me", "4321", nullptr, 0);
  EXPECT_EQ(flags::change_me, 4321);
}

TEST(FlagsCliSet, ChecksFlagsExist) {
  EXPECT_THROW(
    flags_cli_set("this flag does not exist", "foobar", nullptr, 0),
    NotFoundError
  );
}

TEST(FlagsCliSet, DefaultsBooleansToYesValue) {
  flags::change_me_bool = false;
  EXPECT_FALSE(flags_cli_set("change-me-bool", std::nullopt, nullptr, 0));
  EXPECT_EQ(flags::change_me_bool, true);
}

TEST(FlagsCliSet, DoesNotDefaultOthersToYesValue) {
  flags::change_me_str = "";
  int argc = 1;
  const char* argv[] = {"--some-other-arg"};
  EXPECT_THROW(
    flags_cli_set("change-me-str", std::nullopt, argv, argc),
    InvalidArgumentError
  );
}

TEST(FlagsCliSet, NonBooleanTakesNextArgumentAsValue) {
  flags::change_me = 1234;
  int argc = 1;
  const char* argv[] = {"42"};
  EXPECT_TRUE(flags_cli_set("change-me", std::nullopt, argv, argc));
  EXPECT_EQ(flags::change_me, 42);
}

TEST(FlagsCliSet, BooleanDoesNotTakeNextArgumentAsValue) {
  flags::change_me_bool = true;
  int argc = 1;
  const char* argv[] = {"no"};
  EXPECT_FALSE(flags_cli_set("change-me-bool", std::nullopt, argv, argc));
  EXPECT_TRUE(flags::change_me_bool);

  flags::change_me_bool = false;
  const char* argv2[] = {"yes"};
  EXPECT_FALSE(flags_cli_set("change-me-bool", std::nullopt, argv2, argc));
  EXPECT_TRUE(flags::change_me_bool); // Booleans default to true.

  flags::change_me_bool = false;
  const char* argv3[] = {"gibberish"};
  EXPECT_FALSE(flags_cli_set("change-me-bool", std::nullopt, argv3, argc));
  EXPECT_TRUE(flags::change_me_bool); // Booleans default to true.
}

TEST(FlagsCliSet, DisablesWithNo) {
  flags::change_me_bool = true;
  int argc = 0;
  const char* argv[] = {};
  EXPECT_FALSE(flags_cli_set("nochange_me_bool", std::nullopt, argv, argc));
  EXPECT_FALSE(flags::change_me_bool);
}

TEST(FlagsCliSet, DisablesWithNoAndUnderscore) {
  flags::change_me_bool = true;
  int argc = 0;
  const char* argv[] = {};
  EXPECT_FALSE(flags_cli_set("no_change_me_bool", std::nullopt, argv, argc));
  EXPECT_FALSE(flags::change_me_bool);
}

TEST(FlagsCliSet, DisablesWithDisable) {
  flags::enable_me = true;
  int argc = 0;
  const char* argv[] = {};
  EXPECT_FALSE(flags_cli_set("disable_me", std::nullopt, argv, argc));
  EXPECT_FALSE(flags::enable_me);
}

TEST(FlagsCliSet, ParsesCustomTypes) {
  flags::custom_flag = other::CustomType{.x = 0, .y = 1};
  int argc = 0;
  const char* argv[] = {};
  EXPECT_FALSE(flags_cli_set("custom_flag", "2,3", argv, argc));
  EXPECT_EQ(flags::custom_flag.value().x, 2);
  EXPECT_EQ(flags::custom_flag.value().y, 3);

  EXPECT_EQ(flags::custom_flag.default_value_string(), "0,1");
}

}
}
