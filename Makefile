# Thin convenience wrapper around the CMake build. The real build logic lives
# in CMakeLists.txt; this just gives you short `make tidy` / `make format` etc.
#
# Typical flow:
#   make            # configure + build (out-of-source in build/)
#   make tidy       # run clang-tidy using build/compile_commands.json
#   make format     # clang-format every source/header in place
#   make clean

BUILD := build

# All C/C++ sources + headers, found RECURSIVELY (any depth), excluding
# generated/vendored dirs. Used for clang-tidy / clang-format file lists.
SOURCES := $(shell find . -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) \
	-not -path './$(BUILD)/*' -not -path './.git/*' -not -path './third_party/*')

.PHONY: all configure clean tidy format
all: configure
	cmake --build $(BUILD)

# Configure via the preset in CMakePresets.json, which pins the Homebrew LLVM
# toolchain so clang-tidy (also Homebrew LLVM) can resolve system headers.
# compile_commands.json is exported into build/ and picked up by clangd via
# the .clangd "CompilationDatabase: build" entry.
configure:
	cmake --preset default

# Run clang-tidy across the project (sources only; headers are checked via the
# .c/.cpp translation units that include them). Requires `make configure` first.
tidy:
	@command -v run-clang-tidy >/dev/null && run-clang-tidy -p $(BUILD) $(filter %.c %.cpp,$(SOURCES)) || \
		clang-tidy -p $(BUILD) $(filter %.c %.cpp,$(SOURCES))

# Format every C/C++ source + header in place
format:
	clang-format -i $(SOURCES)

clean:
	cmake --build $(BUILD) --target clean