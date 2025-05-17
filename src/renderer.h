/**
 * @file renderer.h
 * @brief Declaration of the Renderer class.
 * @version 1.0
 * @author Your Name
 */
#ifndef RENDERER_H
#define RENDERER_H

#include "html_parser.h"
#include <QString>

/**
 * @class Renderer
 * @brief Renders the DOM tree to a QString for display.
 */
class Renderer {
public:
  /**
   * Renders the DOM tree to a QString.
   * @param node The root node of the DOM tree.
   * @param output The QString to store the rendered content.
   */
  void render(const Node& node, QString* output);
};

#endif