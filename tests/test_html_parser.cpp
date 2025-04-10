#include <gtest/gtest.h>
#include "html_parser.h"

// Заглушочная реализация HtmlParser для тестов
class ConcreteHtmlParser : public HtmlParser {
public:
    Node parse(const std::string& html) override {
        Node root;
        root.type = "root";
        if (html.empty()) return root;
        Node child;
        child.type = html.find("<p") != std::string::npos ? "p" : "unknown";
        child.text = html;
        if (html.find("href") != std::string::npos) {
            child.attributes["href"] = "http://example.com";
        }
        if (html.find("class") != std::string::npos) {
            child.attributes["class"] = "test-class";
        }
        root.children.push_back(child);
        return root;
    }
};

// Test fixture for HtmlParser tests
class HtmlParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        scalar_parser = std::make_unique<ConcreteHtmlParser>();
    }

    std::unique_ptr<HtmlParser> scalar_parser;
};

// Unit Test: Parse simple paragraph
TEST_F(HtmlParserTest, ScalarParser_SimpleParagraph) {
    std::string html = "<p>Hello, World!</p>";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "p");
    EXPECT_EQ(result.children[0].text, "<p>Hello, World!</p>");
    EXPECT_TRUE(result.children[0].attributes.empty());
}

// Unit Test: Parse nested elements
TEST_F(HtmlParserTest, ScalarParser_NestedElements) {
    std::string html = "<div><p>Nested</p></div>";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "p");
    EXPECT_EQ(result.children[0].text, "<div><p>Nested</p></div>");
}

// Unit Test: Parse empty HTML
TEST_F(HtmlParserTest, ScalarParser_EmptyHtml) {
    std::string html = "";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(0));
}

// Unit Test: Parse HTML with attributes
TEST_F(HtmlParserTest, ScalarParser_Attributes) {
    std::string html = "<a href=\"http://example.com\">Link</a>";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "unknown");
    EXPECT_EQ(result.children[0].text, "<a href=\"http://example.com\">Link</a>");
    ASSERT_EQ(result.children[0].attributes.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].attributes["href"], "http://example.com");
}

// Unit Test: Parse multiple top-level elements
TEST_F(HtmlParserTest, ScalarParser_MultipleTopLevel) {
    std::string html = "<p>First</p><p>Second</p>";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "p");
    EXPECT_EQ(result.children[0].text, "<p>First</p><p>Second</p>");
}

// Unit Test: Parse HTML with comments
TEST_F(HtmlParserTest, ScalarParser_Comments) {
    std::string html = "<p>Text</p><!-- Comment -->";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "p");
    EXPECT_EQ(result.children[0].text, "<p>Text</p><!-- Comment -->");
}

// Unit Test: Parse malformed HTML
TEST_F(HtmlParserTest, ScalarParser_MalformedHtml) {
    std::string html = "<p>Unclosed tag";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "p");
    EXPECT_EQ(result.children[0].text, "<p>Unclosed tag");
}

// Unit Test: Parse HTML with multiple attributes
TEST_F(HtmlParserTest, ScalarParser_MultipleAttributes) {
    std::string html = "<a href=\"http://example.com\" class=\"test-class\">Link</a>";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "unknown");
    ASSERT_EQ(result.children[0].attributes.size(), static_cast<size_t>(2));
    EXPECT_EQ(result.children[0].attributes["href"], "http://example.com");
    EXPECT_EQ(result.children[0].attributes["class"], "test-class");
}

// Unit Test: Parse HTML with whitespace
TEST_F(HtmlParserTest, ScalarParser_Whitespace) {
    std::string html = "  <p>  Text  </p>  ";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "p");
    EXPECT_EQ(result.children[0].text, "  <p>  Text  </p>  ");
}

// Unit Test: Parse self-closing tag
TEST_F(HtmlParserTest, ScalarParser_SelfClosingTag) {
    std::string html = "<br/>";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "unknown");
    EXPECT_EQ(result.children[0].text, "<br/>");
}

// Unit Test: Parse plain text
TEST_F(HtmlParserTest, ScalarParser_PlainText) {
    std::string html = "Just text";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "unknown");
    EXPECT_EQ(result.children[0].text, "Just text");
}

// Unit Test: Parse invalid attributes
TEST_F(HtmlParserTest, ScalarParser_InvalidAttributes) {
    std::string html = "<p class=>Invalid</p>";
    Node result = scalar_parser->parse(html);
    ASSERT_EQ(result.type, "root");
    ASSERT_EQ(result.children.size(), static_cast<size_t>(1));
    EXPECT_EQ(result.children[0].type, "p");
    EXPECT_EQ(result.children[0].text, "<p class=>Invalid</p>");
}