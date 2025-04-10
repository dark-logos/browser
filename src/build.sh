#!/bin/bash

# build.sh: Builds the QuickDOM project

# Exit on any error
set -e

# Paths
QMAKE=/opt/homebrew/bin/qmake
EXECUTABLE=build/QuickDOM.app/Contents/MacOS/QuickDOM

echo "Starting build process for QuickDOM..."

# Clean previous build
echo "Cleaning previous build..."
make clean || true # Continue even if make clean fails
rm -f Makefile Makefile.Debug Makefile.Release .qmake.stash .qmake.cache

# Check if browser.pro exists
echo "Checking for browser.pro..."
if [ ! -f "browser.pro" ]; then
    echo "Error: browser.pro not found in current directory"
    exit 1
fi

# Run qmake
echo "Running qmake..."
$QMAKE browser.pro
if [ $? -ne 0 ]; then
    echo "Error: qmake failed"
    exit 1
fi

# Build project
echo "Building project..."
make -j$(sysctl -n hw.ncpu) # Use all CPU cores for faster build
if [ $? -ne 0 ]; then
    echo "Error: make failed"
    exit 1
fi

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable $EXECUTABLE not found"
    exit 1
fi

echo "Build completed successfully. Executable: $EXECUTABLE"