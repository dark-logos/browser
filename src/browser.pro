# Base configuration for Qt
QT += core gui widgets svg testlib
CONFIG += c++17
CONFIG += app_bundle

# Common include paths and libraries
INCLUDEPATH += $$PWD/include

# Platform-specific configurations
macx {
    message("Configuring for macOS")
    INCLUDEPATH += /opt/homebrew/include
    LIBS += -L/opt/homebrew/lib -lcurl
    LIBS += -framework QtWidgets -framework QtSvg -framework QtGui
    LIBS += -framework QtTest -framework QtCore
}

unix:!macx {
    message("Configuring for Linux")
    INCLUDEPATH += /usr/include /usr/local/include
    LIBS += -L/usr/lib -L/usr/local/lib -lcurl
    LIBS += -lQt5Widgets -lQt5Svg -lQt5Gui -lQt5Test -lQt5Core
}

win32 {
    message("Configuring for Windows")
    INCLUDEPATH += $$quote(C:/Program Files/libcurl/include)
    LIBS += -L$$quote(C:/Program Files/libcurl/lib) -lcurl
    # For Qt libraries, rely on Qt's built-in paths on Windows
    # Ensure libcurl is installed (e.g., via vcpkg or manual installation)
}

# Main application target
TEMPLATE = app
TARGET = QuickDOM

# Source and header files for the main application
SOURCES = \
    main.cpp \
    browser_window.cpp \
    html_parser.cpp \
    network.cpp \
    renderer.cpp \
    link_label.cpp

HEADERS = \
    browser_window.h \
    html_parser.h \
    network.h \
    renderer.h \
    link_label.h

# Test configuration
test {
    message("Building tests")
    TEMPLATE = app
    TARGET = QuickDOMTests
    CONFIG += console
    CONFIG -= app_bundle

    # Include test sources
    SOURCES -= main.cpp
    SOURCES += \
        ../tests/main_test.cpp \
        ../tests/test_html_parser.cpp \
        ../tests/test_network.cpp \
        ../tests/test_renderer.cpp \
        ../tests/test_browser_window.cpp \
        ../tests/test_link_label.cpp

    # Google Test dependencies
    macx {
        LIBS += -L/opt/homebrew/lib -lgtest -lgmock -lgtest_main -lgmock_main -pthread
    }
    unix:!macx {
        LIBS += -L/usr/lib -L/usr/local/lib -lgtest -lgmock -lgtest_main -lgmock_main -pthread
    }
    win32 {
        LIBS += -L$$quote(C:/Program Files/googletest/lib) -lgtest -lgmock -lgtest_main -lgmock_main
    }

    # Ensure test executable is built in a separate directory
    DESTDIR = ../tests/build
    OBJECTS_DIR = ../tests/build/obj
    MOC_DIR = ../tests/build/moc
}
else {
    message("Building main application")
    DESTDIR = ./build
    OBJECTS_DIR = ./build/obj
    MOC_DIR = ./build/moc
}