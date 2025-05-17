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
#include <QString>
#include <map>

/**
 * @class BrowserWindow
 * @brief Управляет графическим интерфейсом браузера с вкладками.
 */
class BrowserWindow : public QMainWindow {
  Q_OBJECT

public:
  /**
   * Конструктор окна браузера.
   * @param parent Родительский виджет.
   */
  explicit BrowserWindow(QWidget *parent = nullptr);

private slots:
  /**
   * Открывает новую вкладку с указанным URL.
   */
  void openNewTab();

  /**
   * Обрабатывает переключение вкладок и восстанавливает замороженные вкладки.
   * @param index Индекс активной вкладки.
   */
  void onTabChanged(int index);

private:
  /**
   * Замораживает вкладку, сохраняя её состояние на диск.
   * @param index Индекс вкладки.
   */
  void freezeTab(int index);

  /**
   * Размораживает вкладку, восстанавливая её состояние.
   * @param index Индекс вкладки.
   */
  void unfreezeTab(int index);

  QTabWidget *tabs_;          ///< Управляет вкладками браузера.
  QLineEdit *url_bar_;        ///< Поле ввода URL.
  Network network_;           ///< Модуль сети для загрузки страниц.
  HtmlParser parser_;         ///< Модуль парсинга HTML.
  Renderer renderer_;         ///< Модуль рендеринга.
  std::map<int, QString> frozen_tabs_; ///< Хранит URL замороженных вкладок.
};

#endif  // BROWSER_WINDOW_H