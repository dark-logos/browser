#include <gtest/gtest.h>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "renderer.h"

// Test fixture for Renderer tests
class RendererTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        int argc = 0;
        char* argv[] = {nullptr};
        app = new QApplication(argc, argv);
    }

    static void TearDownTestSuite() {
        delete app;
    }

    void SetUp() override {
        renderer = std::make_unique<Renderer>();
        widget = new QWidget();
        layout = new QVBoxLayout(widget);
    }

    void TearDown() override {
        delete widget;
    }

    static QApplication* app;
    std::unique_ptr<Renderer> renderer;
    QWidget* widget;
    QVBoxLayout* layout;
};

QApplication* RendererTest::app = nullptr;

// Unit Test: Render text node
TEST_F(RendererTest, Render_TextNode) {
    Node node;
    node.type = "text";
    node.text = "Simple text";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 1);
}

// Unit Test: Render paragraph node
TEST_F(RendererTest, Render_ParagraphNode) {
    Node node;
    node.type = "p";
    node.text = "Paragraph content";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 1);
}

// Unit Test: Render image node
TEST_F(RendererTest, Render_ImageNode) {
    Node node;
    node.type = "img";
    node.attributes["src"] = "test.jpg";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 0); // Ожидаем 0, так как изображение не загружается
}

// Unit Test: Render empty link node
TEST_F(RendererTest, Render_EmptyLinkNode) {
    Node node;
    node.type = "a";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 0);
}

// Unit Test: Render image with invalid dimensions
TEST_F(RendererTest, Render_ImageInvalidDimensions) {
    Node node;
    node.type = "img";
    node.attributes["src"] = "test.jpg";
    node.attributes["width"] = "invalid";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 0);
}

// Unit Test: Render span node
TEST_F(RendererTest, Render_SpanNode) {
    Node node;
    node.type = "span";
    node.text = "Span content";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 1);
}

// Unit Test: Render empty node
TEST_F(RendererTest, Render_EmptyNode) {
    Node node;
    node.type = "";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 0);
}

// Unit Test: Render SVG image
TEST_F(RendererTest, Render_SvgImage) {
    Node node;
    node.type = "img";
    node.attributes["src"] = "test.svg";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 0); // Ожидаем 0, так как SVG не загружается
}

// Unit Test: Render node with style attributes
TEST_F(RendererTest, Render_StyleAttributes) {
    Node node;
    node.type = "p";
    node.text = "Styled";
    node.attributes["style"] = "color: blue;";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 1);
}

// Unit Test: Render node with empty attributes
TEST_F(RendererTest, Render_EmptyAttributes) {
    Node node;
    node.type = "p";
    node.text = "Empty attr";
    node.attributes["data-test"] = "";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 1);
}

// Unit Test: Render broken SVG
TEST_F(RendererTest, Render_BrokenSvg) {
    Node node;
    node.type = "img";
    node.attributes["src"] = "broken.svg";
    renderer->render(node, layout);
    EXPECT_EQ(layout->count(), 0);
}

// Unit Test: Render many children
TEST_F(RendererTest, Render_ManyChildren) {
    Node root;
    root.type = "div";
    for (int i = 0; i < 5; ++i) {
        Node child;
        child.type = "p";
        child.text = "Child " + std::to_string(i);
        root.children.push_back(child);
    }
    renderer->render(root, layout);
    EXPECT_EQ(layout->count(), 6); // 5 параграфов + div
}