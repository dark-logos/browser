/**
 * @file renderer.cpp
 * @brief Implementation of the Renderer class.
 * @version 1.0
 * @author dark-logos
 */
#include "renderer.h"

void Renderer::render(const Node& node, QString* output) {
  if (node.tag == "text") {
    *output += QString::fromStdString(node.text) + "\n";
  }
  for (const Node& child : node.children) {
    render(child, output);
  }
}