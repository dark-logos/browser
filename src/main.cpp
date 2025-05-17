/**
 * @file main.cpp
 * @brief Entry point for the mini-browser application.
 * @version 1.0
 * @author dark-logos
 */
#include "browser_window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  BrowserWindow window;
  window.show();
  return app.exec();
}