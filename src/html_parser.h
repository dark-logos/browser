/**
 * @file html_parser.h
 * @brief Declaration of the HtmlParser class and Node struct.
 * @version 1.0
 * @author dark-logos
 */
#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>
#include <vector>

/**
 * @struct Node
 * @brief Represents a node in the DOM tree.
 */
struct Node {
  std::string tag;              ///< The tag name (e.g., "text", "p").
  std::string text;             ///< The text content of the node.
  std::vector<Node> children;   ///< Child nodes in the DOM tree.
};

/**
 * @class HtmlParser
 * @brief Parses HTML content into a DOM tree.
 */
class HtmlParser {
public:
  /**
   * Parses HTML content into a DOM tree.
   * @param html The HTML content to parse.
   * @return The root node of the DOM tree.
   */
  Node parse(const std::string& html);
};

#endif  