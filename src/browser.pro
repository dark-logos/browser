QT += core gui widgets svg
CONFIG += c++17

TARGET = QuickDOM
TEMPLATE = app

SOURCES += \
    main.cpp \
    browser_window.cpp \
    html_parser.cpp \
    network.cpp \
    renderer.cpp \
    link_label.cpp

HEADERS += \
    browser_window.h \
    html_parser.h \
    network.h \
    renderer.h \
    link_label.h

INCLUDEPATH += /opt/homebrew/include
LIBS += -L/opt/homebrew/lib -lcurl