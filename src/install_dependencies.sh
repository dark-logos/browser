#!/bin/bash

# Script to install dependencies for the QuickDOM project (Qt, libcurl, Google Test)
# Works on Linux and Windows (via WSL or Git Bash)

# Exit on error
set -e

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to detect the operating system
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "linux"
    elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
        echo "windows"
    else
        echo "Unsupported OS: $OSTYPE"
        exit 1
    fi
}

# Function to install Qt
install_qt() {
    echo "Installing Qt..."
    if [ "$OS" == "linux" ]; then
        if command_exists apt-get; then
            sudo apt-get update
            sudo apt-get install -y qt5-default qtcreator libqt5svg5-dev
        elif command_exists dnf; then
            sudo dnf install -y qt5-devel qt-creator qt5-qtsvg-devel
        elif command_exists yum; then
            sudo yum install -y qt5-devel qt-creator qt5-qtsvg-devel
        else
            echo "No supported package manager found (apt, dnf, or yum). Please install Qt manually."
            exit 1
        fi
    elif [ "$OS" == "windows" ]; then
        echo "Please download and install Qt from https://www.qt.io/download"
        echo "Recommended: Install Qt 5.15 via Qt Online Installer or use vcpkg."
        echo "To install via vcpkg, run: vcpkg install qt5-base qt5-svg"
        exit 1
    fi
}

# Function to install libcurl
install_libcurl() {
    echo "Installing libcurl..."
    if [ "$OS" == "linux" ]; then
        if command_exists apt-get; then
            sudo apt-get install -y libcurl4-openssl-dev
        elif command_exists dnf; then
            sudo dnf install -y libcurl-devel
        elif command_exists yum; then
            sudo yum install -y libcurl-devel
        else
            echo "No supported package manager found. Please install libcurl manually."
            exit 1
        fi
    elif [ "$OS" == "windows" ]; then
        if command_exists vcpkg; then
            vcpkg install curl
        else
            echo "vcpkg not found. Please install vcpkg (https://github.com/microsoft/vcpkg) and run: vcpkg install curl"
            exit 1
        fi
    fi
}

# Function to install Google Test
install_gtest() {
    echo "Installing Google Test..."
    if [ "$OS" == "linux" ]; then
        if command_exists apt-get; then
            sudo apt-get install -y libgtest-dev
            # Compile Google Test if needed
            if [ ! -f /usr/lib/libgtest.a ]; then
                sudo mkdir -p /usr/src/gtest/build
                sudo cmake -S /usr/src/gtest -B /usr/src/gtest/build
                sudo make -C /usr/src/gtest/build
                sudo cp /usr/src/gtest/build/lib/*.a /usr/lib/
            fi
        elif command_exists dnf; then
            sudo dnf install -y gtest-devel
        elif command_exists yum; then
            sudo yum install -y gtest-devel
        else
            echo "No supported package manager found. Please install Google Test manually."
            exit 1
        fi
    elif [ "$OS" == "windows" ]; then
        if command_exists vcpkg; then
            vcpkg install gtest
        else
            echo "vcpkg not found. Please install vcpkg (https://github.com/microsoft/vcpkg) and run: vcpkg install gtest"
            exit 1
        fi
    fi
}

# Main execution
echo "Starting dependency installation for QuickDOM project..."

# Detect OS
OS=$(detect_os)
echo "Detected OS: $OS"

# Install dependencies
install_qt
install_libcurl
install_gtest

echo "All dependencies installed successfully!"
echo "You can now run 'qmake' and 'make' to build the project."