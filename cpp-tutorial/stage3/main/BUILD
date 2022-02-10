load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc","SharedPtr.h"],
    hdrs = ["hello-greet.h"],
)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc","SharedPtr.h","String.h","String.cc","priority_queue.h","matrix.h","text_query.h","Query.h",
    "Find.h","shared_ptr1.h","my_allocator.h","singleton.h"],
    linkopts=["-lpthread"],
    deps = [
        ":hello-greet",
        "//lib:hello-time",
    ],
)
