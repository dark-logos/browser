/**
 * @file html_parser.h
 * @brief Defines HTML parsing interface and DOM structure.
 */
#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>
#include <map>
#include <vector>

/**
 * @brief Represents a DOM node.
 */
struct Node {
    std::string type; // e.g., "text", "image", "link", "header", "div"
    std::string text; // Text content for text nodes or link text
    std::map<std::string, std::string> attributes; // Tag attributes
    std::vector<Node> children; // Child nodes
};

/**
 * @brief Interface for HTML parsers.
 */
class HtmlParser {
public:
    virtual ~HtmlParser() = default;
    /**
     * @brief Parses HTML into a DOM tree.
     * @param html HTML content.
     * @return Root node of the DOM tree.
     */
    virtual Node parse(const std::string& html) = 0;
};

/**
 * @brief Scalar HTML parser implementation.
 */
class ScalarParser : public HtmlParser {
public:
    Node parse(const std::string& html) override;
};

/**
 * @brief SIMD-accelerated HTML parser for x86.
 */
class SimdParser : public HtmlParser {
public:
    Node parse(const std::string& html) override;
};

/**
 * @brief NEON-accelerated HTML parser for ARM.
 */
class NeonParser : public HtmlParser {
public:
    Node parse(const std::string& html) override;
};

#endif // HTML_PARSER_H