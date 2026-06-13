#!/usr/bin/env python3
import xml.etree.ElementTree as ET
import os
import glob
import sys

def parse_bazel_test_logs(workspace_dir):
    """
    Parses Bazel XML test outputs to generate a high-level CI/CD summary.
    This demonstrates the Python infrastructure scripting required in the JD.
    """
    print("========================================")
    print(" ADAS Build Infrastructure - CI Summary ")
    print("========================================")
    
    # Bazel stores test XML outputs in bazel-testlogs
    testlogs_dir = os.path.join(workspace_dir, 'bazel-testlogs')
    
    if not os.path.exists(testlogs_dir):
        print("No test logs found. Run 'bazel test //...' first.")
        sys.exit(1)

    xml_files = glob.glob(os.path.join(testlogs_dir, '**', 'test.xml'), recursive=True)
    
    total_tests = 0
    total_failures = 0
    
    for xml_file in xml_files:
        try:
            tree = ET.parse(xml_file)
            root = tree.getroot()
            
            # Bazel test suites are usually wrapped in a <testsuites> tag
            tests = int(root.attrib.get('tests', 0))
            failures = int(root.attrib.get('failures', 0))
            errors = int(root.attrib.get('errors', 0))
            
            total_tests += tests
            total_failures += (failures + errors)
            
            target_name = xml_file.replace(testlogs_dir, '').strip('/')
            status = "PASSED" if (failures + errors) == 0 else "FAILED"
            
            print(f"[{status}] {target_name} ({tests} tests)")
            
        except Exception as e:
            print(f"Error parsing {xml_file}: {e}")

    print("----------------------------------------")
    print(f"Total Tests Executed : {total_tests}")
    print(f"Total Failures       : {total_failures}")
    
    if total_failures > 0:
        print("\nSTATUS: CI Pipeline FAILED ❌")
        sys.exit(1)
    else:
        print("\nSTATUS: CI Pipeline PASSED ✅")
        sys.exit(0)

if __name__ == "__main__":
    workspace_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
    parse_bazel_test_logs(workspace_root)
