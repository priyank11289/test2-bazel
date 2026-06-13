#!/bin/bash
# Wrapper to run cppcheck with MISRA addon during Bazel tests

if ! command -v cppcheck &> /dev/null; then
    echo "cppcheck could not be found."
    # If we are in GitHub Actions, we should fail. Otherwise, warn and pass.
    if [ "$GITHUB_ACTIONS" = "true" ]; then
        exit 1
    else
        echo "Skipping MISRA analysis locally. (Will run in CI)"
        exit 0
    fi
fi

echo "Running MISRA analysis on: $@"
cppcheck --language=c++ --error-exitcode=1 --addon=misra -I . "$@"
