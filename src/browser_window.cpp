/**
 * @file browser_window.cpp
 * @brief Implementation of the BrowserWindow class.
 * @version 1.0
 * @author dark-logos
 */
#include "browser_window.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QFont>
#include <QFile>
#include <QTextStream>

BrowserWindow::BrowserWindow(QWidget *parent)
    : QMainWindow(parent),
      parser_(
#if defined(__x86_64__) || defined(__i386__)
          new SimdParser()
#elif defined(__arm64__)
          new NeonParser()
#else
          new ScalarParser()
#endif
      ) {
  // Установка шрифта для устранения предупреждения
  QFont font("Helvetica", 12);
  QApplication::setFont(font);

  // Инициализация UI
  QWidget *central_widget = new QWidget(this);
  setCentralWidget(central_widget);
  QVBoxLayout *layout = new QVBoxLayout(central_widget);

  // Поле для URL
  url_bar_ = new QLineEdit(this);
  url_bar_->setPlaceholderText("Введите URL (например, http://example.com)");
  layout->addWidget(url_bar_);

  // Кнопка открытия вкладки
  QPushButton *open_button = new QPushButton("Открыть вкладку", this);
  connect(open_button, &QPushButton::clicked, this, &BrowserWindow::openNewTab);
  layout->addWidget(open_button);

  // Вкладки
  tabs_ = new QTabWidget(this);
  layout->addWidget(tabs_);
  connect(tabs_, &QTabWidget::currentChanged, this, &BrowserWindow::onTabChanged);

  setWindowTitle("Mini Browser");
  resize(800, 600);
}

void BrowserWindow::openNewTab() {
  // Загрузка и парсинг страницы
  std::string url = url_bar_->text().toStdString();
  std::string html = network_.fetch(url);
  Node root = parser_.parse(html);

  // Создание новой вкладки
  QTextEdit *tab_content = new QTextEdit(this);
  tab_content->setReadOnly(true);
  QString rendered_text;
  renderer_.render(root, &rendered_text);
  tab_content->setText(rendered_text);

  // Добавление вкладки
  int index = tabs_->addTab(tab_content, QString::fromStdString(url));
  frozen_tabs_[index] = QString::fromStdString(url);
}

void BrowserWindow::onTabChanged(int index) {
  // Размораживаем вкладку, если она заморожена
  if (tabs_->widget(index) == nullptr) {
    unfreezeTab(index);
  }
  // Замораживаем неактивные вкладки
  for (int i = 0; i < tabs_->count(); ++i) {
    if (i != index && tabs_->widget(i) != nullptr) {
      freezeTab(i);
    }
  }
}

void BrowserWindow::freezeTab(int index) {
  QTextEdit *tab_content = qobject_cast<QTextEdit*>(tabs_->widget(index));
  if (!tab_content) return;

  // Сохраняем содержимое на диск
  QString url = frozen_tabs_[index];
  QFile file(QString("tab_%1.html").arg(index));
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out << tab_content->toPlainText();
    file.close();
  }

  // Удаляем виджет, освобождая память
  tabs_->removeTab(index);
  tabs_->insertTab(index, new QWidget(), url);
}

void BrowserWindow::unfreezeTab(int index) {
  QString url = frozen_tabs_[index];
  std::string html = network_.fetch(url.toStdString());
  Node root = parser_.parse(html);

  // Восстанавливаем вкладку
  QTextEdit *tab_content = new QTextEdit(this);
  tab_content->setReadOnly(true);
  QString rendered_text;
  renderer_.render(root, &rendered_text);
  tab_content->setText(rendered_text);

  // Заменяем пустой виджет на восстановленный
  tabs_->removeTab(index);
  tabs_->insertTab(index, tab_content, url);
}