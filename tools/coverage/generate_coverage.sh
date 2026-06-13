#!/bin/bash
set -e

echo "==========================================="
echo " Running Bazel Code Coverage Analysis... "
echo "==========================================="

# Run Bazel coverage over the entire workspace
# --combined_report=lcov ensures it outputs a standard lcov tracefile
bazel coverage //... --combined_report=lcov

# Check if lcov/genhtml is installed on the machine
if ! command -v genhtml &> /dev/null; then
    echo "ERROR: genhtml (from lcov) is not installed."
    echo "Please install it with: sudo apt-get install lcov"
    exit 1
fi

echo "Generating visual HTML report..."
mkdir -p coverage_report

# Bazel outputs the combined coverage file to bazel-out/_coverage/_coverage_report.dat
genhtml bazel-out/_coverage/_coverage_report.dat -o coverage_report

echo "==========================================="
echo " Coverage report generated successfully!"
echo " You can view it by opening: coverage_report/index.html"
echo "==========================================="
