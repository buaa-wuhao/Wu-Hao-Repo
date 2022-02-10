load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],
)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc","Pub_sub.h","virtual_function.h"],
    linkopts=["-lpthread"],
    deps = [
        ":hello-greet",
    ],
)
