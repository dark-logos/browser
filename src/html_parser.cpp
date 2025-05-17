/**
 * @file html_parser.cpp
 * @brief Implementation of the HtmlParser class.
 * @version 1.0
 * @author dark-logos
 */
#include "html_parser.h"

Node HtmlParser::parse(const std::string& html) {
  Node root;
  root.tag = "root";
  // Simplified, extracts text between <p> tags
  size_t pos = 0;
  while ((pos = html.find("<p>", pos)) != std::string::npos) {
    size_t end = html.find("</p>", pos);
    if (end == std::string::npos) break;
    Node child;
    child.tag = "text";
    child.text = html.substr(pos + 3, end - pos - 3);
    root.children.push_back(child);
    pos = end + 4;
  }
  return root;
}