# Qt project file
QT += core gui widgets
TARGET = QuickDOM
TEMPLATE = app
SOURCES += main.cpp browser_window.cpp network.cpp html_parser.cpp renderer.cpp
HEADERS += browser_window.h network.h html_parser.h renderer.h
LIBS += -lcurl
CONFIG += c++20