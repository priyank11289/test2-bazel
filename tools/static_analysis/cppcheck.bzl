def misra_check(name, srcs):
    """
    A custom Bazel macro to run Cppcheck with the MISRA C++ addon.
    """
    srcs_str = " ".join(["$(location %s)" % s for s in srcs])
    native.genrule(
        name = name,
        srcs = srcs + ["//tools/static_analysis:run_cppcheck.sh"],
        outs = [name + ".passed"],
        cmd = "bash $(location //tools/static_analysis:run_cppcheck.sh) " + srcs_str + " && touch $@",
    )
