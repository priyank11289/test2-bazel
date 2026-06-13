load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("//tools/static_analysis:cppcheck.bzl", "misra_check")

cc_library(
    name = "speed_monitor",
    srcs = ["speed_monitor.cpp"],
    hdrs = ["speed_monitor.h"],
    deps = ["//common:math_utils"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "adas_app",
    srcs = ["main.cpp"],
    deps = [":speed_monitor"],
)

misra_check(
    name = "misra_check",
    srcs = [
        "main.cpp",
        "speed_monitor.cpp",
        "speed_monitor.h",
        "//common:math_utils.cpp",
        "//common:math_utils.h",
    ],
)
