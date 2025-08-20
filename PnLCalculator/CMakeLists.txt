# CMakeList.txt : CMake project for PnLCalculator, include source and define
# project specific logic here.
#

# Add source to this project's executable.
cmake_minimum_required(VERSION 3.10)
project(PnLCalculator)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
set(SOURCES
    src/main.cpp
    src/PnLCalculator.cpp
)

# Create executable
add_executable(PnLCalculator ${SOURCES})

# Use C++17 specifically for this target
target_compile_features(PnLCalculator PRIVATE cxx_std_17)

# Include directories for all targets
include_directories(include)

# --- Google Test setup ---
# Enable testing
enable_testing()

# Download and configure GoogleTest automatically
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(pnl_tests "tests/testPnLCalculator.cpp" src/PnLCalculator.cpp)

# Link Google Test libraries
target_link_libraries(pnl_tests gtest_main)

# Register tests
include(GoogleTest)
gtest_discover_tests(pnl_tests)
