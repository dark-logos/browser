/**
 * @file browser_window.cpp
 * @brief Implementation of the BrowserWindow class.
 * @version 1.0
 * @author dark-logos
 */
#include "browser_window.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>

BrowserWindow::BrowserWindow(QWidget *parent) : QMainWindow(parent) {
  QWidget *central_widget = new QWidget(this);
  setCentralWidget(central_widget);
  QVBoxLayout *layout = new QVBoxLayout(central_widget);

  url_bar_ = new QLineEdit(this);
  url_bar_->setPlaceholderText("Enter URL (e.g., http://example.com)");
  layout->addWidget(url_bar_);

  QPushButton *open_button = new QPushButton("Open Tab", this);
  connect(open_button, &QPushButton::clicked, this, &BrowserWindow::openNewTab);
  layout->addWidget(open_button);

  tabs_ = new QTabWidget(this);
  layout->addWidget(tabs_);

  setWindowTitle("Mini Browser");
  resize(800, 600);
}

void BrowserWindow::openNewTab() {
  std::string url = url_bar_->text().toStdString();
  std::string html = network_.fetch(url);
  Node root = parser_.parse(html);

  // new tab
  QTextEdit *tab_content = new QTextEdit(this);
  tab_content->setReadOnly(true);
  QString rendered_text;
  renderer_.render(root, &rendered_text);
  tab_content->setText(rendered_text);

  tabs_->addTab(tab_content, QString::fromStdString(url));
}