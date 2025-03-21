#!/bin/bash

# build_run.sh: Builds and runs the QuickDOM project

# Exit on any error
set -e

# Paths
QMAKE=/opt/homebrew/bin/qmake
CERT_PEM=/opt/homebrew/etc/openssl@3/cert.pem
EXECUTABLE=QuickDOM.app/Contents/MacOS/QuickDOM

echo "Starting build process..."

# Clean previous build
echo "Cleaning previous build..."
make clean || true # Continue even if make clean fails
rm -f Makefile Makefile.Debug Makefile.Release .qmake.stash .qmake.cache

# Run qmake
echo "Running qmake..."
if [ ! -f "browser.pro" ]; then
    echo "Error: browser.pro not found in current directory"
    exit 1
fi
$QMAKE browser.pro
if [ $? -ne 0 ]; then
    echo "Error: qmake failed"
    exit 1
fi

# Build project
echo "Building project..."
make
if [ $? -ne 0 ]; then
    echo "Error: make failed"
    exit 1
fi

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable $EXECUTABLE not found"
    exit 1
fi

# Check if certificate file exists
if [ ! -f "$CERT_PEM" ]; then
    echo "Warning: Certificate file $CERT_PEM not found, SSL verification may fail"
fi

# Run the application
echo "Running QuickDOM..."
export CURL_CA_BUNDLE=$CERT_PEM
./$EXECUTABLE

echo "Application exited."