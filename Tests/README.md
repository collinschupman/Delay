# Delay Plugin Unit Tests

This directory contains unit tests for the Delay audio plugin using Google Test framework.

## Prerequisites

- CMake 3.14 or higher
- C++14 compatible compiler
- Wwise SDK installed with `WWISESDK` environment variable set

## Building Tests

```bash
cd Tests
mkdir build
cd build
cmake ..
cmake --build .
```

## Running Tests

After building:

```bash
./delay_tests
```

Or use CTest:

```bash
ctest --output-on-failure
```

## Test Coverage

The test suite covers:

- **CircularBuffer**: Write/read operations, wraparound behavior, delay time updates
- **Delayline**: Initialization, delayed read/write, linear interpolation
- **Helpers**: Linear interpolation (lerp), parameter smoothing, range mapping (jmap)
- **LFO**: Phase progression, frequency changes, sine wave generation, phase offsets

## Adding New Tests

1. Create a new test file: `test_<component>.cpp`
2. Add it to `CMakeLists.txt` in the `add_executable` section
3. Include necessary headers from `../SoundEnginePlugin`
4. Use Google Test macros: `TEST()`, `TEST_F()`, `EXPECT_*`, `ASSERT_*`

## Test Structure

Tests use the Arrange-Act-Assert pattern:
- **Arrange**: Set up test fixtures and initial conditions
- **Act**: Execute the code under test
- **Assert**: Verify expected outcomes using EXPECT/ASSERT macros
