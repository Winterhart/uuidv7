# SPDX-License-Identifier: MIT
# UUIDv7 - RFC 9562
# Copyright (c) 2026 Farfatech Inc.
# Conan producer recipe: builds this library into a Conan package.
#
# One-time machine setup:
#   conan profile detect --force
# Build the package (also builds+runs test_package as a consumer smoke test):
#   conan create .
# Afterwards any project on this machine can depend on it:
#   conan install --requires=uuidv7/0.1

import os
import re

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.files import load


class UUIDv7Conan(ConanFile):
    name = "uuidv7"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain"
    # Source files that ship with the package build. The build reuses the
    # project's own CMakeLists.txt; Conan wraps, not replaces, the build.
    exports_sources = "CMakeLists.txt", "include/*", "src/*"

    def set_version(self):
        # Single source of truth: project() in CMakeLists.txt. The regex is
        # anchored on the project() line so cmake_minimum_required(VERSION ...)
        # further up the file cannot match.
        cmake_lists = load(self, os.path.join(self.recipe_folder, "CMakeLists.txt"))
        self.version = re.search(r"project\(UUIDv7\s+VERSION\s+([0-9.]+)", cmake_lists).group(1)

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        # The packaging machine does not need Criterion: skip the test suite.
        cmake.configure(variables={"UUIDV7_BUILD_TESTS": False})
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        # How the package presents itself to CMake consumers:
        #   find_package(UUIDv7 REQUIRED)  ->  target UUIDv7::UUIDv7
        self.cpp_info.set_property("cmake_file_name", "UUIDv7")
        self.cpp_info.set_property("cmake_target_name", "UUIDv7::UUIDv7")
        self.cpp_info.libs = ["UUIDv7"]
