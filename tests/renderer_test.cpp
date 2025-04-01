#include <gtest/gtest.h>
#include "renderer.h"
#include <QApplication>
#include <QVBoxLayout>

TEST(RendererTest, RenderTextNode) {
    int argc = 0;
    QApplication app(argc, nullptr);
    Renderer renderer;
    Node node;
    node.type = "p";
    node.text = "Test paragraph";
    QVBoxLayout layout;
    renderer.render(node, &layout);
    ASSERT_EQ(layout.count(), 1);
    QLabel* label = qobject_cast<QLabel*>(layout.itemAt(0)->widget());
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Test paragraph");
}

TEST(RendererTest, RenderHeaderNode) {
    int argc = 0;
    QApplication app(argc, nullptr);
    Renderer renderer;
    Node node;
    node.type = "header";
    node.text = "Test Header";
    QVBoxLayout layout;
    renderer.render(node, &layout);
    ASSERT_EQ(layout.count(), 1);
    QLabel* label = qobject_cast<QLabel*>(layout.itemAt(0)->widget());
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Test Header");
    EXPECT_TRUE(label->font().bold());
}

TEST(RendererTest, RenderLinkNode) {
    int argc = 0;
    QApplication app(argc, nullptr);
    Renderer renderer;
    Node node;
    node.type = "link";
    node.text = "Click me";
    node.attributes["href"] = "https://example.com";
    QVBoxLayout layout;
    renderer.render(node, &layout);
    ASSERT_EQ(layout.count(), 1);
    QLabel* label = qobject_cast<QLabel*>(layout.itemAt(0)->widget());
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Click me");
    EXPECT_TRUE(label->property("isLink").toBool());
    EXPECT_EQ(label->property("href").toString().toStdString(), "https://example.com");
}

TEST(RendererTest, RenderEmptyNode) {
    int argc = 0;
    QApplication app(argc, nullptr);
    Renderer renderer;
    Node node;
    node.type = "unknown";
    QVBoxLayout layout;
    renderer.render(node, &layout);
    EXPECT_EQ(layout.count(), 0);
}