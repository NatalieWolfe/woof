load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
  name = "scheduler",
  hdrs = ["scheduler.h"],
  srcs = ["scheduler.cpp"],
)

cc_test(
  name = "scheduler_test",
  srcs = ["scheduler_test.cpp"],
  deps = [
    ":scheduler",
    "@gtest//:gtest_main",
  ],
)
