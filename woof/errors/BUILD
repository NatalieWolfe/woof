load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
  name = "canonical",
  visibility = ["//visibility:public"],
  hdrs = ["canonical.h"],
  deps = [":error"],
)

cc_library(
  name = "error",
  hdrs = ["error.h"],
  srcs = ["error.cpp"],
  deps = [
    "//woof/io/stream:buffer",
  ],
)

cc_test(
  name = "error_test",
  srcs = ["error_test.cpp"],
  deps = [
    ":error",
    "@gtest//:gtest_main",
  ],
)
