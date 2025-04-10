#!/bin/bash

# Script to build and run QuickDOM tests

set -e

SRC_DIR="$(pwd)"
TEST_DIR="$(dirname "$SRC_DIR")/tests"
BUILD_DIR="$TEST_DIR/build"
TEST_BINARY="$BUILD_DIR/QuickDOMTests"

# Ensure src and test directories exist
if [ ! -d "$SRC_DIR" ]; then
    echo "Error: Source directory $SRC_DIR does not exist."
    exit 1
fi

if [ ! -d "$TEST_DIR" ]; then
    echo "Error: Test directory $TEST_DIR does not exist."
    exit 1
fi

# Create build directory
mkdir -p "$BUILD_DIR"

# Navigate to source directory
cd "$SRC_DIR"

# Run qmake for test configuration
echo "Running qmake for test configuration..."
qmake CONFIG+=test browser.pro

# Build the tests with verbose output
echo "Building tests..."
make -j$(sysctl -n hw.ncpu) V=1

# Check if test binary exists
if [ ! -f "$TEST_BINARY" ]; then
    echo "Error: Test binary $TEST_BINARY not found."
    exit 1
fi

# Run the tests
echo "Running tests..."
"$TEST_BINARY"

# Clean up (optional)
echo "Cleaning up..."
make clean
rm -f Makefile