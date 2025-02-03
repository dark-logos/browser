/**
 * @file renderer.h
 * @brief Defines renderer for DOM to Qt widgets.
 */
#ifndef RENDERER_H
#define RENDERER_H

#include "html_parser.h"
#include <QWidget>
#include <QVBoxLayout>

/**
 * @class Renderer
 * @brief Renders DOM tree into Qt widgets.
 */
class Renderer {
public:
  /**
   * @brief Renders DOM tree into a layout.
   * @param node DOM tree root node.
   * @param layout Qt layout to populate.
   */
  void render(const Node& node, QVBoxLayout* layout);
};

#endif