/**
 * @file renderer.h
 * @brief Defines renderer for DOM nodes.
 */
#ifndef RENDERER_H
#define RENDERER_H

#include "html_parser.h"
#include <QVBoxLayout>

/**
 * @class Renderer
 * @brief Renders DOM nodes into Qt widgets.
 */
class Renderer {
public:
    /**
     * @brief Renders a DOM node into a layout.
     * @param node DOM node to render.
     * @param layout Target layout.
     */
    void render(const Node& node, QVBoxLayout* layout);
};

#endif