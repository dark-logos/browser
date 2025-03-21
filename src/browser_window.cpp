/**
 * @file browser_window.cpp
 * @brief Implements browser GUI and tab management.
 */
#include "browser_window.h"
#include "link_label.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QFile>
#include <QTextStream>
#include <QPalette>

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
    // Set dark theme
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::white);
    setPalette(palette);

    QFont font("Arial", 10);
    QApplication::setFont(font);

    auto* central_widget = new QWidget(this);
    central_widget->setStyleSheet("background: black;");
    setCentralWidget(central_widget);
    auto* layout = new QVBoxLayout(central_widget);

    url_bar_ = new QLineEdit(this);
    url_bar_->setPlaceholderText("Enter URL (e.g., http://example.com)");
    url_bar_->setStyleSheet("background: #333; color: white; border: 1px solid #555;");
    layout->addWidget(url_bar_);

    auto* open_button = new QPushButton("Open Tab", this);
    open_button->setStyleSheet("background: #444; color: white;");
    connect(open_button, &QPushButton::clicked, this, &BrowserWindow::openNewTab);
    layout->addWidget(open_button);

    tabs_ = new QTabWidget(this);
    tabs_->setStyleSheet("QTabWidget { background: black; color: white; } QTabBar::tab { background: #333; color: white; }");
    layout->addWidget(tabs_);
    connect(tabs_, &QTabWidget::currentChanged, this, &BrowserWindow::onTabChanged);

    setWindowTitle("QuickDOM");
    resize(800, 600);
}

void BrowserWindow::openNewTab() {
    std::string url = url_bar_->text().toStdString();
    std::string html = network_.fetch(url);
    Node root = parser_->parse(html);

    // Load media
    for (auto& child : root.children) {
        if (child.type == "image") {
            auto src_it = child.attributes.find("src");
            if (src_it != child.attributes.end()) {
                std::string media_path = network_.fetchMedia(src_it->second, url);
                if (!media_path.empty()) {
                    child.attributes["src"] = media_path;
                }
            }
        }
    }

    auto* scroll_area = new QScrollArea(this);
    scroll_area->setStyleSheet("QScrollArea { background: black; }");
    auto* content_widget = new QWidget();
    content_widget->setStyleSheet("background: black;");
    auto* content_layout = new QVBoxLayout(content_widget);
    content_layout->setAlignment(Qt::AlignTop);
    renderer_.render(root, content_layout);

    // Connect link clicks
    for (int i = 0; i < content_layout->count(); ++i) {
        auto* item = content_layout->itemAt(i);
        if (item && item->widget()) {
            if (auto* label = qobject_cast<QLabel*>(item->widget())) {
                if (label->property("isLink").toBool()) {
                    auto* link_label = new LinkLabel(content_widget);
                    link_label->setText(label->text());
                    link_label->setStyleSheet(label->styleSheet());
                    link_label->setProperty("href", label->property("href"));
                    connect(link_label, &LinkLabel::clicked, this, &BrowserWindow::handleLinkClicked);
                    content_layout->replaceWidget(label, link_label);
                    label->deleteLater();
                }
            }
        }
    }

    scroll_area->setWidget(content_widget);
    scroll_area->setWidgetResizable(true);

    int index = tabs_->addTab(scroll_area, QString::fromStdString(url));
    frozen_tabs_[index] = QString::fromStdString(url);
}

void BrowserWindow::handleLinkClicked(QLabel* label) {
    QString href = label->property("href").toString();
    if (!href.isEmpty()) {
        url_bar_->setText(href);
        openNewTab();
    }
}

void BrowserWindow::onTabChanged(int index) {
    if (tabs_->widget(index) == nullptr) {
        unfreezeTab(index);
    }
    for (int i = 0; i < tabs_->count(); ++i) {
        if (i != index && tabs_->widget(i) != nullptr) {
            freezeTab(i);
        }
    }
}

void BrowserWindow::freezeTab(int index) {
    auto* scroll_area = qobject_cast<QScrollArea*>(tabs_->widget(index));
    if (!scroll_area) return;

    QString url = frozen_tabs_[index];
    QFile file(QString("tab_%1.html").arg(index));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << url;
        file.close();
    }

    tabs_->removeTab(index);
    tabs_->insertTab(index, new QWidget(), url);
}

void BrowserWindow::unfreezeTab(int index) {
    QString url = frozen_tabs_[index];
    std::string html = network_.fetch(url.toStdString());
    Node root = parser_->parse(html);

    // Load media
    for (auto& child : root.children) {
        if (child.type == "image") {
            auto src_it = child.attributes.find("src");
            if (src_it != child.attributes.end()) {
                std::string media_path = network_.fetchMedia(src_it->second, url.toStdString());
                if (!media_path.empty()) {
                    child.attributes["src"] = media_path;
                }
            }
        }
    }

    auto* scroll_area = new QScrollArea(this);
    scroll_area->setStyleSheet("QScrollArea { background: black; }");
    auto* content_widget = new QWidget();
    content_widget->setStyleSheet("background: black;");
    auto* content_layout = new QVBoxLayout(content_widget);
    content_layout->setAlignment(Qt::AlignTop);
    renderer_.render(root, content_layout);

    // Connect link clicks
    for (int i = 0; i < content_layout->count(); ++i) {
        auto* item = content_layout->itemAt(i);
        if (item && item->widget()) {
            if (auto* label = qobject_cast<QLabel*>(item->widget())) {
                if (label->property("isLink").toBool()) {
                    auto* link_label = new LinkLabel(content_widget);
                    link_label->setText(label->text());
                    link_label->setStyleSheet(label->styleSheet());
                    link_label->setProperty("href", label->property("href"));
                    connect(link_label, &LinkLabel::clicked, this, &BrowserWindow::handleLinkClicked);
                    content_layout->replaceWidget(label, link_label);
                    label->deleteLater();
                }
            }
        }
    }

    scroll_area->setWidget(content_widget);
    scroll_area->setWidgetResizable(true);

    tabs_->removeTab(index);
    tabs_->insertTab(index, scroll_area, url);
}