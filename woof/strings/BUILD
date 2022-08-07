load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
  name = "format",
  visibility = ["//visibility:public"],
  hdrs = ["format.h"],
  srcs = ["format.cpp"],
  deps = ["//woof/errors:canonical"],
)

cc_test(
  name = "format_test",
  srcs = ["format_test.cpp"],
  deps = [
    ":format",
    "@gtest//:gtest_main",
    "//woof/errors:canonical",
  ],
)
