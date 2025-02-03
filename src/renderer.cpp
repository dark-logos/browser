/**
 * @file renderer.cpp
 * @brief Implements DOM rendering to Qt widgets.
 */
#include "renderer.h"
#include <QLabel>
#include <QPixmap>
#include <iostream>

void Renderer::render(const Node& node, QVBoxLayout* layout) {
  if (node.type == "text") {
    QLabel* label = new QLabel(QString::fromStdString(node.text));
    label->setWordWrap(true);
    layout->addWidget(label);
    std::cout << "Rendering text: " << node.text << "\n";
  } else if (node.type == "image") {
    auto src_it = node.attributes.find("src");
    if (src_it != node.attributes.end()) {
      QLabel* image_label = new QLabel();
      QPixmap pixmap(QString::fromStdString(src_it->second));
      if (!pixmap.isNull()) {
        // Scale image if width/height provided
        auto width_it = node.attributes.find("width");
        auto height_it = node.attributes.find("height");
        int width = width_it != node.attributes.end() ? std::stoi(width_it->second) : pixmap.width();
        int height = height_it != node.attributes.end() ? std::stoi(height_it->second) : pixmap.height();
        image_label->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio));
        layout->addWidget(image_label);
        std::cout << "Rendering image: " << src_it->second << "\n";
      } else {
        std::cerr << "Failed to load image: " << src_it->second << "\n";
      }
    }
  }

  for (const auto& child : node.children) {
    render(child, layout);
  }
}