load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
  name = "cli",
  hdrs = ["cli.h"],
  srcs = ["cli.cpp"],
  deps = [
    ":flag_base",
    ":registry",
    "//woof/errors:canonical",
  ],
)

cc_test(
  name = "cli_test",
  srcs = ["cli_test.cpp"],
  deps = [
    ":cli",
    ":test_flags",
    "@gtest//:gtest_main",
    "//woof/errors:canonical",
  ],
)

cc_library(
  name = "flag_base_h",
  hdrs = ["flag_base.h"],
)

cc_library(
  name = "flag_base",
  srcs = ["flag_base.cpp"],
  deps = [
    ":flag_base_h",
    ":registry",
  ],
)

cc_library(
  name = "flags",
  visibility = ["//visibility:public"],
  hdrs = ["flags.h"],
  srcs = ["flags.cpp"],
  deps = [
    ":flag_base",
    ":registry",
    "//woof/strings:format",
  ],
)

cc_test(
  name = "flags_test",
  srcs = ["flags_test.cpp"],
  deps = [
    ":flags",
    ":test_flags",
    "@gtest//:gtest_main",
  ],
)

cc_library(
  name = "init_flags",
  visibility = ["//woof/service:__subpackages__"],
  hdrs = ["init_flags.h"],
  srcs = ["init_flags.cpp"],
  deps = [
    ":cli",
    ":flags",
    "//woof/errors:canonical",
    "//woof/errors:macros",
  ],
)

cc_test(
  name = "init_flags_test",
  srcs = ["init_flags_test.cpp"],
  deps = [
    ":init_flags",
    "@gtest//:gtest_main",
    "//woof/errors:canonical",
  ]
)

cc_library(
  name = "print_flags",
  visibility = ["//visibility:public"],
  hdrs = ["print_flags.h"],
  srcs = ["print_flags.cpp"],
  deps = [
    ":flag_base",
    ":registry",
  ],
)

cc_test(
  name = "print_flags_test",
  srcs = ["print_flags_test.cpp"],
  deps = [
    ":print_flags",
    ":test_flags",
    "@gtest//:gtest_main",
  ],
)

cc_library(
  name = "registry",
  hdrs = ["registry.h"],
  srcs = ["registry.cpp"],
  deps = [
    ":flag_base_h",
    "//woof/errors:canonical",
  ],
)

cc_library(
  name = "test_flags",
  testonly = True,
  hdrs = ["test_flags.h"],
  deps = [
    ":flags",
    "//woof/strings:format",
  ],
)
