cmake_minimum_required(VERSION 3.20)
project(UUIDv7 VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile_commands.json so clangd / clang-tidy know your flags & includes.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enforce c11 instead of gnu11 for portability
set(CMAKE_C_EXTENSIONS OFF )


if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Warnings that apply to every target.
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
    -Wformat=2
    -Wundef
    # -Werror is intentionally off; turn it on in CI when you're ready.
)

# ---- Targets ---------------------------------------------------------------
# Add new executables/libraries here. Each .c/.cpp gets its own entry.
add_library(UUIDv7 src/UUIDv7.c)
target_include_directories(UUIDv7 PUBLIC include)
# C++ target
# add_executable(hello_cpp hello.cpp)
# target_compile_features(hello_cpp PRIVATE cxx_std_20)

# ---- Testing ---------------------------------------------------------------
# Criterion is installed via Homebrew, which ships no CMake config package
# (CriterionConfig.cmake), only a pkg-config .pc file. So we discover it via
# the pkg-config protocol: find_package(PkgConfig) + pkg_check_modules.
find_package(PkgConfig REQUIRED)
pkg_check_modules(CRITERION REQUIRED IMPORTED_TARGET criterion)

# Any top-level CMakeLists with tests calls this once.
enable_testing()

add_executable(uuidv7_tests tests/test_UUIDv7.c)
target_link_libraries(uuidv7_tests PRIVATE UUIDv7 PkgConfig::CRITERION)

# Register the executable with CTest: "a test is a process; exit code is truth."
add_test(NAME uuidv7_tests COMMAND uuidv7_tests)
