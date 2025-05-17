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
 * @brief Представляет узел в дереве DOM.
 */
struct Node {
  std::string tag;              ///< Имя тега (например, "text", "p").
  std::string text;             ///< Текстовое содержимое узла.
  std::vector<Node> children;   ///< Дочерние узлы в дереве DOM.
};

/**
 * @class ParserStrategy
 * @brief Абстрактный базовый класс для стратегий парсинга HTML.
 */
class ParserStrategy {
public:
  virtual ~ParserStrategy() = default;
  /**
   * Парсит HTML-контент в дерево DOM.
   * @param html HTML-контент для парсинга.
   * @return Корневой узел дерева DOM.
   */
  virtual Node parse(const std::string& html) = 0;
};

/**
 * @class ScalarParser
 * @brief Скалярная реализация парсинга HTML.
 */
class ScalarParser : public ParserStrategy {
public:
  Node parse(const std::string& html) override;
};

#if defined(__x86_64__) || defined(__i386__)
/**
 * @class SimdParser
 * @brief SSE4.2-ускоренная реализация парсинга HTML для x86/x64.
 */
class SimdParser : public ParserStrategy {
public:
  Node parse(const std::string& html) override;
};
#elif defined(__arm64__)
/**
 * @class NeonParser
 * @brief NEON-ускоренная реализация парсинга HTML для ARM.
 */
class NeonParser : public ParserStrategy {
public:
  Node parse(const std::string& html) override;
};
#endif

/**
 * @class HtmlParser
 * @brief Парсит HTML-контент в дерево DOM с использованием указанной стратегии.
 */
class HtmlParser {
public:
  /**
   * Конструктор HtmlParser с указанной стратегией парсинга.
   * @param strategy Указатель на стратегию парсинга (например, ScalarParser, SimdParser или NeonParser).
   */
  explicit HtmlParser(ParserStrategy* strategy) : strategy_(strategy) {}

  /**
   * Парсит HTML-контент в дерево DOM.
   * @param html HTML-контент для парсинга.
   * @return Корневой узел дерева DOM.
   */
  Node parse(const std::string& html) { return strategy_->parse(html); }

private:
  ParserStrategy* strategy_;  ///< Стратегия парсинга.
};

#endif  // HTML_PARSER_H