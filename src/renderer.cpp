/**
 * @file renderer.cpp
 * @brief Renders DOM tree into Qt widgets.
 */
#include "renderer.h"
#include <QLabel>
#include <QPixmap>
#include <QSvgRenderer>
#include <QPainter>
#include <iostream>

void Renderer::render(const Node& node, QVBoxLayout* layout) {
  if (node.type == "text") {
    QLabel* label = new QLabel(QString::fromStdString(node.text));
    label->setWordWrap(true);
    layout->addWidget(label);
    std::cout << "Rendering text: " << node.text << "\n";
  } else if (node.type == "image") {
    auto src_it = node.attributes.find("src");
    if (src_it != node.attributes.end() && !src_it->second.empty()) {
      try {
        QPixmap pixmap;
        // Check if file is SVG
        if (src_it->second.find(".svg") != std::string::npos) {
          QSvgRenderer svg_renderer(QString::fromStdString(src_it->second));
          if (svg_renderer.isValid()) {
            pixmap = QPixmap(100, 100); // Default size for SVG
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
          // Scale image if width/height specified
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
          // Limit size to prevent memory issues
          width = std::min(width, 800);
          height = std::min(height, 600);
          image_label->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio));
          layout->addWidget(image_label);
          std::cout << "Rendering image: " << src_it->second << "\n";
        } else {
          std::cerr << "Failed to load pixmap: " << src_it->second << "\n";
        }
      } catch (const std::exception& e) {
        std::cerr << "Error rendering image " << src_it->second << ": " << e.what() << "\n";
      }
    } else {
      std::cerr << "Missing or empty src for image\n";
    }
  }

  for (const auto& child : node.children) {
    render(child, layout);
  }
}