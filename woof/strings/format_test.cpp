#include "woof/strings/format.h"

#include <chrono>
#include <string>

#include "gtest/gtest.h"
#include "woof/errors/canonical.h"

namespace woof {
namespace {

TEST(Format, Bool) {
  EXPECT_EQ(format(true), "true");
  EXPECT_EQ(format(false), "false");
}

TEST(Parse, BoolTrue) {
  EXPECT_TRUE(parse<bool>("true"));
  EXPECT_TRUE(parse<bool>("yes"));
  EXPECT_TRUE(parse<bool>("1"));
}

TEST(Parse, BoolFalse) {
  EXPECT_FALSE(parse<bool>("false"));
  EXPECT_FALSE(parse<bool>("no"));
  EXPECT_FALSE(parse<bool>("0"));
}

TEST(Parse, BoolInvalid) {
  EXPECT_THROW(parse<bool>("gibberish"), InvalidArgumentError);
}

// -------------------------------------------------------------------------- //

TEST(Format, Int) {
  EXPECT_EQ(format((unsigned short)8080), "8080");
  EXPECT_EQ(format(-321), "-321");
}

TEST(Parse, Int) {
  EXPECT_EQ(parse<unsigned short>("8080"), 8080);
  EXPECT_EQ(parse<int>("-321"), -321);
}

TEST(Parse, IntInvalid) {
  EXPECT_THROW(parse<unsigned short>("1000000"), InvalidArgumentError);
  EXPECT_THROW(parse<unsigned int>("-321"), InvalidArgumentError);
  EXPECT_THROW(parse<int>("123 gibberish"), InvalidArgumentError);
  EXPECT_THROW(parse<long long int>("gibberish"), InvalidArgumentError);
}

// -------------------------------------------------------------------------- //

TEST(Format, Float) {
  // TODO: Find a better way to encode floats to remove this grossness.
  EXPECT_EQ(format(123.456f), "123.456001"); // Floats are bad, m'kay.
  EXPECT_EQ(format(-654.321), "-654.321000");
}

TEST(Parse, Float) {
  EXPECT_EQ(parse<float>("123.456"), 123.456f);
  EXPECT_EQ(parse<double>("-654.321"), -654.321);
  EXPECT_EQ(parse<long double>("4.321E4"), 43210);
}

TEST(Parse, FloatInvalid) {
  EXPECT_THROW(parse<float>("4E+38"), InvalidArgumentError);
  EXPECT_THROW(parse<double>("123 gibberish"), InvalidArgumentError);
  EXPECT_THROW(parse<long double>("gibberish"), InvalidArgumentError);
}

// -------------------------------------------------------------------------- //

TEST(Format, String) {
  EXPECT_EQ(format("foobar"), "foobar");
  EXPECT_EQ(format("fizz\\bang"), "fizz\\\\bang");
  EXPECT_EQ(format("gleep\"glorp"), "gleep\\\"glorp");
}

TEST(Parse, String) {
  EXPECT_EQ(parse<std::string>("foobar"), "foobar");
  EXPECT_EQ(parse<std::string>("fizz\\\\bang"), "fizz\\bang");
  EXPECT_EQ(parse<std::string>("gleep\\\"glorp"), "gleep\"glorp");
}

// -------------------------------------------------------------------------- //

TEST(Format, Duration) {
  using namespace std::chrono;
  EXPECT_EQ(format(nanoseconds(1234)), "1234ns");
  EXPECT_EQ(format(milliseconds(15)), "15ms");
  EXPECT_EQ(format(milliseconds(1000 * 60)), "1m");
  EXPECT_EQ(format(seconds(17)), "17s");
  EXPECT_EQ(format(seconds(60)), "1m");
  EXPECT_EQ(format(hours(48)), "2d");

  // Year is ~25 seconds longer than 365 days.
  EXPECT_EQ(format(days(365)), "365d");
  EXPECT_EQ(format(days(366)), "366d");
  EXPECT_EQ(format(years(9001)), "9001y");
}

TEST(Parse, Duration) {
  using namespace std::chrono;
  EXPECT_EQ(parse<nanoseconds>("3y"), years(3));
  EXPECT_EQ(parse<nanoseconds>("365d"), days(365));
  EXPECT_EQ(parse<nanoseconds>("24h"), hours(24));
  EXPECT_EQ(parse<nanoseconds>("60m"), minutes(60));
  EXPECT_EQ(parse<nanoseconds>("60s"), seconds(60));
  EXPECT_EQ(parse<nanoseconds>("1000ms"), milliseconds(1000));
  EXPECT_EQ(parse<nanoseconds>("123us"), microseconds(123));
  EXPECT_EQ(parse<nanoseconds>("15ns"), nanoseconds(15));
}

TEST(Parse, ComplexDuration) {
  using namespace std::chrono;
  EXPECT_EQ(
    parse<nanoseconds>("1h2m3s4ms"),
    hours(1) + minutes(2) + seconds(3) + milliseconds(4)
  );
}

}
}
