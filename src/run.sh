#!/bin/bash

# run.sh: Runs the QuickDOM project

# Exit on any error
set -e

# Paths
CERT_PEM=/opt/homebrew/etc/openssl@3/cert.pem
EXECUTABLE=QuickDOM.app/Contents/MacOS/QuickDOM

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