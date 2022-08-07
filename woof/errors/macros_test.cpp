#include "woof/errors/macros.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "woof/errors/canonical.h"

namespace woof {
namespace {

using testing::MatchesRegex;

TEST(ErrorMacros, NullCheckThrowsOnNull) {
  try {
    void* foo = nullptr;
    void* bar = &foo;
    WOOF_CHECK_NULL(InvalidArgumentError, bar); // Does not throw.
    WOOF_CHECK_NULL(InvalidArgumentError, foo); // Does throw.
  } catch (const InvalidArgumentError& err) {
    EXPECT_THAT(
      err.what(), MatchesRegex(".*foo must not be null.*")
    );
  } catch (...) {
    FAIL() << "Should throw InvalidArgumentError.";
  }
}

TEST(ErrorMacros, CheckGreaterThan) {
  try {
    int small = 1;
    int big = 42;
    WOOF_CHECK_GT(FailedPreconditionError, big, small) << "Does not throw.";
    WOOF_CHECK_GT(FailedPreconditionError, small, big) << "Does throw.";
  } catch (const FailedPreconditionError& err) {
    EXPECT_THAT(
      err.what(),
      MatchesRegex(
        ".*`small` \\(1\\) to be greater than `big` \\(42\\).*Does throw."
      )
    );
  } catch (...) {
    FAIL() << "Should throw FailedPreconditionError.";
  }
}

TEST(ErrorMacros, CheckGreaterThanOrEqualTo) {
  try {
    int small = 1;
    int big = 42;
    WOOF_CHECK_GTE(FailedPreconditionError, big, small) << "Does not throw.";
    WOOF_CHECK_GTE(FailedPreconditionError, big, big) << "Does not throw.";
    WOOF_CHECK_GTE(FailedPreconditionError, small, big) << "Does throw.";
  } catch (const FailedPreconditionError& err) {
    EXPECT_THAT(
      err.what(),
      MatchesRegex(
        ".*`small` \\(1\\) to be greater than or equal to `big` \\(42\\).*"
        "Does throw."
      )
    );
  } catch (...) {
    FAIL() << "Should throw FailedPreconditionError.";
  }
}

}
}
