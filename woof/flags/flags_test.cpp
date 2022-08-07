#include "woof/flags/flags.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "woof/flags/test_flags.h"

namespace woof {
namespace {

TEST(FlagsExists, FalseForFlagsThatAreNotRegistered) {
  EXPECT_FALSE(flag_exists("this flag does not exist"));
}

TEST(FlagsExists, TrueForFlagsThatAreRegistered) {
  EXPECT_TRUE(flag_exists("bool_true"));
}

TEST(FlagsExists, ConvertsUnderscoresToDashes) {
  // Flag was registered with _, but we're testing with a -.
  EXPECT_TRUE(flag_exists("bool-true"));
}

// -------------------------------------------------------------------------- //

TEST(FlagsInstances, ConstructorMakesFlagsExist) {
  EXPECT_FALSE(flag_exists("this flag is created in this test"));
  Flag<int> flag{"", "this flag is created in this test", 1, ""};
  EXPECT_TRUE(flag_exists("this flag is created in this test"));
}

TEST(FlagsInstances, ConstructorConvertsUnderscoresToDashes) {
  EXPECT_FALSE(flag_exists("this-flag-is-created-in-this-test"));
  Flag<int> flag{"", "this-flag-is-created-in-this-test", 1, ""};
  EXPECT_TRUE(flag_exists("this-flag-is-created-in-this-test"));
  EXPECT_TRUE(flag_exists("this_flag_is_created_in_this_test"));
}

TEST(FlagsInstances, ImplicitlyConvertibleToValue) {
  EXPECT_TRUE(flags::bool_true);
  EXPECT_FALSE(flags::bool_false);
  EXPECT_EQ(flags::answer_to_life, 42);
  EXPECT_FLOAT_EQ(flags::pi, 3.14);
  EXPECT_DOUBLE_EQ(flags::moar_pi, 3.1416);
  EXPECT_EQ(flags::str, "Lorem ipsum");
}

TEST(FlagsInstances, CanChangeValue) {
  flags::change_me = 1234;
  EXPECT_EQ(flags::change_me, 1234);
  flags::change_me = 4321;
  EXPECT_EQ(flags::change_me, 4321);
}

}
}
