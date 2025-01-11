/**
 * @file browser_window.h
 * @brief Declaration of the BrowserWindow class, managing the browser's GUI.
 * @version 1.0
 * @author dark-logos
 */
#ifndef BROWSER_WINDOW_H
#define BROWSER_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include "network.h"
#include "html_parser.h"
#include "renderer.h"

class BrowserWindow : public QMainWindow {
  Q_OBJECT

public:
  /**
   * Constructs the browser window.
   * @param parent The parent widget.
   */
  explicit BrowserWindow(QWidget *parent = nullptr);

private slots:
  /**
   * Handles the action of opening a new tab with the specified URL.
   */
  void openNewTab();

private:
  QTabWidget *tabs_;          ///< Manages browser tabs.
  QLineEdit *url_bar_;        ///< Input field for URLs.
  Network network_;           ///< Network module for fetching pages.
  HtmlParser parser_;         ///< HTML parser module.
  Renderer renderer_;         ///< Renderer module.
};

#endif 