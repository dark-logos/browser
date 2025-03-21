/**
 * @file browser_window.h
 * @brief Defines browser GUI and tab management.
 */
#ifndef BROWSER_WINDOW_H
#define BROWSER_WINDOW_H

#include "html_parser.h"
#include "network.h"
#include "renderer.h"
#include <QMainWindow>
#include <QLineEdit>
#include <QTabWidget>
#include <QLabel> // Added for QLabel
#include <QMap>

class BrowserWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit BrowserWindow(QWidget *parent = nullptr);

private slots:
    void openNewTab();
    void onTabChanged(int index);
    void handleLinkClicked(QLabel* label); // Handle link clicks

private:
    void freezeTab(int index);
    void unfreezeTab(int index);

    QLineEdit* url_bar_;
    QTabWidget* tabs_;
    QMap<int, QString> frozen_tabs_;
    Network network_;
    Renderer renderer_;
    std::unique_ptr<HtmlParser> parser_;
};

#endif // BROWSER_WINDOW_H