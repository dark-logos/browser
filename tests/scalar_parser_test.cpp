#include <gtest/gtest.h>
#include "html_parser.h"

TEST(ScalarParserTest, ParseSimpleParagraph) {
    ScalarParser parser;
    std::string html = "<p>Hello, World!</p>";
    Node root = parser.parse(html);
    ASSERT_EQ(root.type, "root");
    ASSERT_EQ(root.children.size(), 1);
    EXPECT_EQ(root.children[0].type, "p");
    EXPECT_EQ(root.children[0].text, "Hello, World!");
    EXPECT_TRUE(root.children[0].attributes.empty());
}

TEST(ScalarParserTest, ParseImageWithAttributes) {
    ScalarParser parser;
    std::string html = "<img src=\"image.jpg\" width=\"100\" height=\"200\" alt=\"test\">";
    Node root = parser.parse(html);
    ASSERT_EQ(root.type, "root");
    ASSERT_EQ(root.children.size(), 1);
    EXPECT_EQ(root.children[0].type, "image");
    EXPECT_EQ(root.children[0].attributes["src"], "image.jpg");
    EXPECT_EQ(root.children[0].attributes["width"], "100");
    EXPECT_EQ(root.children[0].attributes["height"], "200");
    EXPECT_EQ(root.children[0].attributes["alt"], "test");
    EXPECT_TRUE(root.children[0].text.empty());
}

TEST(ScalarParserTest, ParseLinkWithHref) {
    ScalarParser parser;
    std::string html = "<a href=\"https://example.com\">Click me</a>";
    Node root = parser.parse(html);
    ASSERT_EQ(root.type, "root");
    ASSERT_EQ(root.children.size(), 1);
    EXPECT_EQ(root.children[0].type, "link");
    EXPECT_EQ(root.children[0].text, "Click me");
    EXPECT_EQ(root.children[0].attributes["href"], "https://example.com");
}

TEST(ScalarParserTest, ParseNestedDiv) {
    ScalarParser parser;
    std::string html = "<div><p>Nested</p></div>";
    Node root = parser.parse(html);
    ASSERT_EQ(root.type, "root");
    ASSERT_EQ(root.children.size(), 1);
    EXPECT_EQ(root.children[0].type, "div");
    ASSERT_EQ(root.children[0].children.size(), 1);
    EXPECT_EQ(root.children[0].children[0].type, "p");
    EXPECT_EQ(root.children[0].children[0].text, "Nested");
}

TEST(ScalarParserTest, HandleMalformedHTML) {
    ScalarParser parser;
    std::string html = "<p>Unclosed tag <img src=\"test.jpg\">";
    Node root = parser.parse(html);
    ASSERT_EQ(root.type, "root");
    ASSERT_EQ(root.children.size(), 2);
    EXPECT_EQ(root.children[0].type, "p");
    EXPECT_EQ(root.children[0].text, "Unclosed tag ");
    EXPECT_EQ(root.children[1].type, "image");
    EXPECT_EQ(root.children[1].attributes["src"], "test.jpg");
}

TEST(ScalarParserTest, HandleEmptyHTML) {
    ScalarParser parser;
    std::string html = "";
    Node root = parser.parse(html);
    ASSERT_EQ(root.type, "root");
    EXPECT_TRUE(root.children.empty());
}

TEST(ScalarParserTest, HandleInvalidTag) {
    ScalarParser parser;
    std::string html = "<invalid>Content</invalid>";
    Node root = parser.parse(html);
    ASSERT_EQ(root.type, "root");
    EXPECT_TRUE(root.children.empty()); // Unsupported tags are skipped
}