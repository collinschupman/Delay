cmake_minimum_required(VERSION 3.14)
project(DelayTests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/../SoundEnginePlugin)

# Create test executable
add_executable(
  delay_tests
  test_circular_buffer.cpp
  test_delayline.cpp
  test_helpers.cpp
  test_lfo.cpp
  test_simd.cpp
  ../SoundEnginePlugin/CircularBuffer.cpp
  ../SoundEnginePlugin/Delayline.cpp
  ../SoundEnginePlugin/LFO.cpp
)

target_link_libraries(
  delay_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(delay_tests)
