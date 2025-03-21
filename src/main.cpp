/**
 * @file main.cpp
 * @brief Entry point for QuickDOM browser.
 */
#include <QApplication>
#include "browser_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    BrowserWindow window;
    window.show();
    return app.exec();
}