/**
 * @file browser_window.h
 * @brief Defines browser GUI with tab management.
 */
#ifndef BROWSER_WINDOW_H
#define BROWSER_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QScrollArea>
#include "network.h"
#include "html_parser.h"
#include "renderer.h"
#include <QString>
#include <map>

/**
 * @class BrowserWindow
 * @brief Manages browser GUI with tabs and content rendering.
 */
class BrowserWindow : public QMainWindow {
  Q_OBJECT

public:
  /**
   * @brief Constructs browser window.
   * @param parent Parent widget.
   */
  explicit BrowserWindow(QWidget *parent = nullptr);

private slots:
  /**
   * @brief Opens a new tab with the entered URL.
   */
  void openNewTab();

  /**
   * @brief Handles tab switching and frozen tab restoration.
   * @param index Index of the active tab.
   */
  void onTabChanged(int index);

private:
  /**
   * @brief Freezes a tab, saving its state to disk.
   * @param index Tab index.
   */
  void freezeTab(int index);

  /**
   * @brief Restores a frozen tab.
   * @param index Tab index.
   */
  void unfreezeTab(int index);

  QTabWidget *tabs_;
  QLineEdit *url_bar_;
  Network network_;
  HtmlParser parser_;
  Renderer renderer_;
  std::map<int, QString> frozen_tabs_;
};

#endif
