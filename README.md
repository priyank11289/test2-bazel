# Toyota ADAS Build Infrastructure Project

This repository serves as a professional demonstration of an enterprise-grade Build & CI/CD Infrastructure designed for safety-critical automotive software (ADAS). It utilizes **Bazel 9 (Bzlmod)**, integrates strict **MISRA C++** static analysis, enforces **Code Coverage**, and automates deployment via **GitHub Actions**.

---

## 📁 Repository Architecture & File Index

### 1. Build System Configuration (Bazel)
*   **`MODULE.bazel`**: The modern Bazel configuration file (Bzlmod). Replaces the legacy `WORKSPACE` file. It handles fetching external dependencies like `rules_cc` and `googletest`.
*   **`.bazelversion`**: Locks the Bazel version (9.1.1) to ensure hermetic and reproducible builds across all developer machines and CI runners.
*   **`.bazelrc`**: Configures global Bazel behavior. Enforces strict safety-critical compiler flags (`-Wall -Wextra -Werror`), enables local disk caching to drastically reduce compile times, and configures console output formatting.

### 2. Application Source Code (C++)
*   **`adas/BUILD`**: The Bazel build file for the core Advanced Driver Assistance System logic. Defines the `cc_library`, `cc_binary`, and ties in the unit tests and static analysis macros.
*   **`adas/main.cpp`**, **`adas/speed_monitor.cpp`**, **`adas/speed_monitor.h`**: The business logic simulating an ADAS speed violation detection module.
*   **`common/BUILD`**, **`common/math_utils.*`**: A shared utility library demonstrating granular modularity in a Bazel monorepo. Kept separate so changes here don't invalidate the cache of unrelated modules.

### 3. Testing & Coverage
*   **`adas/speed_monitor_test.cpp`**, **`common/math_utils_test.cpp`**: Unit tests utilizing Google Test.
*   **`tools/coverage/generate_coverage.sh`**: A shell script that runs `bazel coverage //...` to instrument the code and uses `lcov/genhtml` to generate a visual, navigable HTML report of untested lines.

### 4. Static Analysis (MISRA C++)
*   **`tools/static_analysis/cppcheck.bzl`**: A custom Bazel macro using a `native.genrule`. It integrates MISRA static analysis directly into the Bazel dependency graph. If static analysis fails, the build fails.
*   **`tools/static_analysis/run_cppcheck.sh`**: The underlying bash wrapper that executes the `cppcheck` binary with the MISRA C++ addon and proper include paths.

### 5. Infrastructure Automation
*   **`tools/automation/parse_test_results.py`**: A Python script demonstrating infrastructure automation. It crawls Bazel's hidden XML test logs, parses the XML trees, and outputs a clean, readable CI summary. In production, this drives Slack/Jira notifications.
*   **`.vscode/c_cpp_properties.json`**: Developer Experience (DevEx) configuration. Tells VS Code IntelliSense exactly where Bazel hides downloaded dependencies (like Google Test headers) to prevent false-positive errors in the IDE.

### 6. Continuous Integration & Delivery
*   **`.github/workflows/bazel-ci.yml`**: The central CI/CD orchestrator.

---

## 🔄 The CI/CD Pipeline Flow

When a developer pushes code to `master` (or opens a Pull Request), the GitHub Actions pipeline executes the following deterministic flow:

1.  **Environment Setup**: Spins up an Ubuntu runner, checks out the code, and installs necessary system tools (e.g., `cppcheck`).
2.  **Cache Restoration**: Pulls the previous Bazel cache (`~/.cache/bazel`) to skip recompiling code that hasn't changed. This is the cornerstone of scaling massive monorepos.
3.  **Run Tests & Static Analysis (`bazel test //...`)**: 
    *   Executes all Google Test binaries.
    *   Executes the custom `misra_check` targets. If any developer introduced a MISRA violation or a failing test, the pipeline halts immediately.
4.  **Compilation (`bazel build //...`)**: Compiles the final release-ready `adas_app` binary.
5.  **Artifact Archival**: Uploads the raw binary as a workflow artifact for temporary access.
6.  **Automated Release**: If the push was to the `master` branch, it dynamically creates a new GitHub Release (e.g., `build-42`) and publishes the executable to the repository's Releases page.

---

## 🎯 Interview Q&A Guide (Woven by Toyota)

**Q1: Why did you choose Bazel for this project instead of CMake?**
**A:** Bazel is designed for massive, multi-language monorepos. Unlike CMake, Bazel provides true hermeticity (reproducible builds regardless of the host machine state) and advanced caching. By utilizing remote caching and Remote Build Execution (RBE), Bazel can distribute compile steps across a server farm, reducing a 1-hour ADAS compile down to minutes. 

**Q2: How do you enforce safety standards like MISRA C++ in a Bazel build?**
**A:** You integrate it directly into the Bazel action graph. In this project, I wrote a custom macro (`cppcheck.bzl`) that uses a `genrule` to wrap the static analysis tool. I made the static analysis a "test" dependency. This means developers can't successfully run a build or merge code without Bazel successfully executing the MISRA validation. 

**Q3: How do you speed up the CI/CD pipeline for a massive codebase?**
**A:** Three main strategies:
1. **Granularity**: Breaking large targets into tiny `cc_library` units so a small change only invalidates a tiny fraction of the cache.
2. **Caching**: Implementing local disk caching (seen in my `.bazelrc`) and scaling up to Remote Caching in CI so workers share compiled artifacts.
3. **Hermeticity**: Locking down toolchains and dependency versions (via Bzlmod) so cache hits are guaranteed across different environments.

**Q4: What is Bzlmod and why are you using it instead of a `WORKSPACE` file?**
**A:** Bzlmod is Bazel's modern dependency management system (introduced in Bazel 6, default in 9). The old `WORKSPACE` file suffered from "dependency hell"—if two libraries required different versions of Google Test, it broke. Bzlmod uses a module graph and version resolution (similar to npm or Cargo) to intelligently resolve transitive dependencies, making third-party library management much safer.

**Q5: The JD mentions "Developing python scripts for infrastructure automation." How did you approach that?**
**A:** I built `parse_test_results.py`. Bazel outputs raw, dense XML files deep in its cache (`bazel-testlogs`). Build engineers need to extract actionable data from these tools to feed into dashboards or alerting systems. My script parses these XML trees and translates them into a clear CI pipeline summary.

**Q6: What compile flags are critical for ADAS development?**
**A:** Strictness is key. In my `.bazelrc`, I enforced `-Wall`, `-Wextra`, and specifically `-Werror`. In automotive software, a compiler warning often represents undefined behavior or a memory leak. `-Werror` forces the compiler to treat every warning as a hard failure, preventing bad code from ever reaching the hardware.
