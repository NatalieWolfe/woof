load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
  name = "main",
  visibility = ["//visibility:public"],
  srcs = ["main"],
  deps = [
    ":server",
    ":registry",
    "//woof/flags",
    "//woof/flags:init_flags",
    "//woof/flags:print_flags",
  ],
)
