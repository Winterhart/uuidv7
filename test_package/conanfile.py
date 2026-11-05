# SPDX-License-Identifier: MIT
# UUIDv7 - RFC 9562
# Consumer smoke test for the Conan package. `conan create .` builds the
# package into the local cache, then builds and runs THIS project against
# the cached package — verifying include paths, the CMake target name and
# the API exactly as installed, not as they sit in the source tree.

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout


class UUIDv7TestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        # Replaced by conan create with the package reference it just built.
        self.requires(self.tested_reference_str)

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        cmake = CMake(self)
        cmake.test()
