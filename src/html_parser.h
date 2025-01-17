/**
 * @file html_parser.h
 * @brief Defines HTML parser and DOM node structure.
 */
#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>
#include <vector>
#include <map>

/**
 * @struct Node
 * @brief Represents a DOM tree node.
 */
struct Node {
  std::string type;  // text, image, video, audio
  std::string text;  // content for text nodes
  std::map<std::string, std::string> attributes;  // e.g., src, alt
  std::vector<Node> children;
};

/**
 * @class ParserStrategy
 * @brief Abstract base for HTML parsing strategies.
 */
class ParserStrategy {
public:
  virtual ~ParserStrategy() = default;
  /**
   * @brief Parses HTML into a DOM tree.
   * @param html HTML content to parse.
   * @return Root node of the DOM tree.
   */
  virtual Node parse(const std::string& html) = 0;
};

/**
 * @class ScalarParser
 * @brief Scalar HTML parsing implementation.
 */
class ScalarParser : public ParserStrategy {
public:
  Node parse(const std::string& html) override;
};

#if defined(__x86_64__) || defined(__i386__)
/**
 * @class SimdParser
 * @brief SSE4.2-accelerated HTML parsing for x86/x64.
 */
class SimdParser : public ParserStrategy {
public:
  Node parse(const std::string& html) override;
};
#elif defined(__arm64__)
/**
 * @class NeonParser
 * @brief NEON-accelerated HTML parsing for ARM.
 */
class NeonParser : public ParserStrategy {
public:
  Node parse(const std::string& html) override;
};
#endif

/**
 * @class HtmlParser
 * @brief Parses HTML using a specified strategy.
 */
class HtmlParser {
public:
  /**
   * @brief Constructs parser with a strategy.
   * @param strategy Parsing strategy (e.g., ScalarParser, NeonParser).
   */
  explicit HtmlParser(ParserStrategy* strategy) : strategy_(strategy) {}
  /**
   * @brief Parses HTML into a DOM tree.
   * @param html HTML content to parse.
   * @return Root node of the DOM tree.
   */
  Node parse(const std::string& html) { return strategy_->parse(html); }

private:
  ParserStrategy* strategy_;
};

#endif