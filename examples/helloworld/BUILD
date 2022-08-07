load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_proto_grpc//cpp:defs.bzl", "cpp_grpc_library")
load("//defs.bzl", "woof_service_library")

proto_library(
  name = "hello_proto",
  srcs = ["hello.proto"],
)

cpp_grpc_library(
  name = "hello_cpp_grpc",
  protos = [":hello_proto"],
)

cc_binary(
  name = "service",
  srcs = ["service.cpp"],
  deps = [
    "@com_github_grpc_grpc//:grpc++",
    ":hello_cpp_grpc",
  ],
)

cc_binary(
  name = "client",
  srcs = ["client.cpp"],
  deps = [
    "@com_github_grpc_grpc//:grpc++",
    ":hello_cpp_grpc",
  ],
)

woof_service_library(
  name = "hello_woof_service",
  deps = ["hello_proto"],
)

cc_library(
  name = "greeter_hello_handler",
  srcs = ["woof_server.cpp"],
  alwayslink = True,
  deps = [
    ":hello_woof_service"
    "//woof/async:future",
    "//woof/service:register",
  ],
)

cc_binary(
  name = "woof_server",
  deps = [
    ":greeter_hello_handler",
    "//woof/service:main",
  ],
)
