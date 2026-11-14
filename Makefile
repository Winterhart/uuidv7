# Thin convenience wrapper around the CMake build. The real build logic lives
# in CMakeLists.txt; this just gives you short `make test` / `make tidy` etc.
#
# Typical flow:
#   make            # configure + build (out-of-source in build/)
#   make test       # build then run the test suite through CTest
#   make tidy       # run clang-tidy using build/compile_commands.json
#   make format     # clang-format every source/header in place
#   make clean

BUILD := build

# Pick the configure preset for this machine. macOS pins Homebrew's clang so
# build/compile_commands.json matches the Homebrew clang-tidy that `make tidy`
# runs (see CMakePresets.json). Other Unix systems use the portable `default`
# preset with whatever CC the environment provides.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
PRESET := macos-homebrew-llvm
else
PRESET := default
endif

# All C/C++ sources + headers, found RECURSIVELY (any depth), excluding
# generated/vendored dirs. Used for clang-tidy / clang-format file lists.
SOURCES := $(shell find . -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) \
	-not -path './$(BUILD)/*' -not -path './.git/*' -not -path './third_party/*' \
	-not -path './test_package/*')

.PHONY: all configure clean tidy format test
all: configure
	cmake --build $(BUILD)

# Configure via the preset selected above.
configure:
	cmake --preset $(PRESET)

# Build then run the test suite through CTest.
test: all
	ctest --test-dir $(BUILD) --verbose --output-on-failure

# Run clang-tidy across the project (sources only; headers are checked via the
# .c/.cpp translation units that include them). Requires `make configure` first.
tidy:
	@command -v run-clang-tidy >/dev/null && run-clang-tidy -p $(BUILD) $(filter %.c %.cpp,$(SOURCES)) || \
		clang-tidy -p $(BUILD) $(filter %.c %.cpp,$(SOURCES))

# Format every C/C++ source and header in place
format:
	clang-format -i $(SOURCES)

clean:
	cmake --build $(BUILD) --target clean
