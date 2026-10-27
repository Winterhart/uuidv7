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
add_executable(hello src/hello.c)

# C++ target
# add_executable(hello_cpp hello.cpp)
# target_compile_features(hello_cpp PRIVATE cxx_std_20)

# ---- Convenience: pretty list of sources ----------------------------------
# `cmake --build build --target help` shows these.
enable_testing()
