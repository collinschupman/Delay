#!/bin/bash

# Build and run unit tests for Delay plugin

set -e  # Exit on error

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== Building Delay Plugin Unit Tests ===${NC}\n"

# Check if WWISESDK is set
if [ -z "$WWISESDK" ]; then
    echo -e "${RED}Error: WWISESDK environment variable is not set${NC}"
    echo "Please set it to your Wwise SDK installation path"
    echo "Example: export WWISESDK=/path/to/Wwise/SDK"
    exit 1
fi

echo -e "${GREEN}Using Wwise SDK: $WWISESDK${NC}\n"

# Create build directory
cd "$(dirname "$0")"
mkdir -p build
cd build

# Configure with CMake
echo -e "${BLUE}Configuring...${NC}"
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build
echo -e "\n${BLUE}Building...${NC}"
cmake --build .

# Run tests
if [ "$1" == "--no-run" ]; then
    echo -e "\n${GREEN}Build complete! Skipping test execution.${NC}"
    echo "To run tests manually: cd build && ./delay_tests"
else
    echo -e "\n${BLUE}Running tests...${NC}\n"
    ./delay_tests
    
    echo -e "\n${GREEN}All tests passed!${NC}"
fi
