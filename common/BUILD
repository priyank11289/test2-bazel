load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

exports_files(["math_utils.cpp", "math_utils.h"])

cc_library(
    name = "math_utils",
    srcs = ["math_utils.cpp"],
    hdrs = ["math_utils.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "math_utils_test",
    srcs = ["math_utils_test.cpp"],
    deps = [
        ":math_utils",
        "@googletest//:gtest_main",
    ],
)
