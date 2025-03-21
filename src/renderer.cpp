/**
 * @file renderer.cpp
 * @brief Renders DOM tree into Qt widgets.
 */
#include "renderer.h"
#include <QLabel>
#include <QPixmap>
#include <QSvgRenderer>
#include <QPainter>
#include <QApplication>
#include <iostream>

void Renderer::render(const Node& node, QVBoxLayout* layout) {
    if (node.type == "text" || node.type == "p" || node.type == "div" || node.type == "span") {
        QLabel* label = new QLabel(QString::fromStdString(node.text));
        label->setWordWrap(true);
        label->setStyleSheet("color: white; font-size: 14px;"); // White text
        layout->addWidget(label);
        std::cout << "Rendering text: " << node.text << "\n";
    } else if (node.type == "header") {
        QLabel* label = new QLabel(QString::fromStdString(node.text));
        label->setWordWrap(true);
        label->setStyleSheet("color: white; font-size: 18px; font-weight: bold;"); // White, bold header
        layout->addWidget(label);
        std::cout << "Rendering header: " << node.text << "\n";
    } else if (node.type == "image") {
        auto src_it = node.attributes.find("src");
        if (src_it != node.attributes.end() && !src_it->second.empty()) {
            try {
                QPixmap pixmap;
                if (src_it->second.find(".svg") != std::string::npos) {
                    QSvgRenderer svg_renderer(QString::fromStdString(src_it->second));
                    if (svg_renderer.isValid()) {
                        int width = 100, height = 100; // Default SVG size
                        auto width_it = node.attributes.find("width");
                        auto height_it = node.attributes.find("height");
                        if (width_it != node.attributes.end() && !width_it->second.empty()) {
                            width = std::stoi(width_it->second);
                        }
                        if (height_it != node.attributes.end() && !height_it->second.empty()) {
                            height = std::stoi(height_it->second);
                        }
                        pixmap = QPixmap(width, height);
                        pixmap.fill(Qt::transparent);
                        QPainter painter(&pixmap);
                        svg_renderer.render(&painter);
                    } else {
                        std::cerr << "Invalid SVG: " << src_it->second << "\n";
                        return;
                    }
                } else {
                    pixmap.load(QString::fromStdString(src_it->second));
                }

                if (!pixmap.isNull()) {
                    QLabel* image_label = new QLabel();
                    int width = pixmap.width();
                    int height = pixmap.height();
                    auto width_it = node.attributes.find("width");
                    auto height_it = node.attributes.find("height");
                    if (width_it != node.attributes.end() && !width_it->second.empty()) {
                        try {
                            width = std::stoi(width_it->second);
                        } catch (const std::exception& e) {
                            std::cerr << "Invalid width: " << width_it->second << "\n";
                        }
                    }
                    if (height_it != node.attributes.end() && !height_it->second.empty()) {
                        try {
                            height = std::stoi(height_it->second);
                        } catch (const std::exception& e) {
                            std::cerr << "Invalid height: " << height_it->second << "\n";
                        }
                    }
                    width = std::min(width, 800);
                    height = std::min(height, 600);
                    image_label->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio));
                    layout->addWidget(image_label);
                    std::cout << "Rendering image: " << src_it->second << "\n";
                } else {
                    QLabel* placeholder = new QLabel("Image not loaded");
                    placeholder->setStyleSheet("color: white; background: gray; padding: 5px;");
                    layout->addWidget(placeholder);
                    std::cerr << "Failed to load pixmap: " << src_it->second << "\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "Error rendering image " << src_it->second << ": " << e.what() << "\n";
            }
        }
    } else if (node.type == "link") {
        auto href_it = node.attributes.find("href");
        if (href_it != node.attributes.end() && !href_it->second.empty() && !node.text.empty()) {
            QLabel* link_label = new QLabel(QString::fromStdString(node.text));
            link_label->setWordWrap(true);
            link_label->setStyleSheet("color: #00008B; text-decoration: underline;"); // Dark blue links
            link_label->setCursor(Qt::PointingHandCursor);
            // Store href as property
            link_label->setProperty("href", QString::fromStdString(href_it->second));
            // Connect click event (handled in BrowserWindow)
            link_label->setProperty("isLink", true);
            layout->addWidget(link_label);
            std::cout << "Rendering link: " << node.text << " (" << href_it->second << ")\n";
        }
    }

    for (const auto& child : node.children) {
        render(child, layout);
    }
}