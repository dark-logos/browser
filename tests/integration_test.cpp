#include <gtest/gtest.h>
#include "html_parser.h"
#include "network.h"
#include "renderer.h"
#include "browser_window.h"
#include <QApplication>
#include <QVBoxLayout>
#include <filesystem>

namespace fs = std::filesystem;

TEST(IntegrationTest, FetchParseRenderPipeline) {
    int argc = 0;
    QApplication app(argc, nullptr);
    Network network;
    ScalarParser parser;
    Renderer renderer;

    std::string html = R"(
        <div>
            <h1>Welcome</h1>
            <p>Test content</p>
            <a href="https://example.com">Link</a>
            <img src="test.jpg" width="100" height="100">
        </div>
    )";
    Node root = parser.parse(html);
    ASSERT_EQ(root.type, "root");
    ASSERT_EQ(root.children.size(), 1);
    EXPECT_EQ(root.children[0].type, "div");
    ASSERT_EQ(root.children[0].children.size(), 3);
    EXPECT_EQ(root.children[0].children[0].type, "header");
    EXPECT_EQ(root.children[0].children[1].type, "p");
    EXPECT_EQ(root.children[0].children[2].type, "link");

    QVBoxLayout layout;
    renderer.render(root, &layout);
    EXPECT_EQ(layout.count(), 3); // Header, paragraph, link
}

TEST(IntegrationTest, MediaFetchAndRender) {
    int argc = 0;
    QApplication app(argc, nullptr);
    Network network;
    ScalarParser parser;
    Renderer renderer;

    // Create a fake image file
    fs::create_directory("cache");
    std::string cache_file = "cache/1234567890.media";
    std::ofstream file(cache_file, std::ios::binary);
    file << "fake image data";
    file.close();

    std::string html = "<img src=\"test.jpg\" width=\"100\" height=\"100\">";
    std::string base_url = "https://example.com/";
    std::string cached_path = network.fetchMedia("test.jpg", base_url);
    EXPECT_EQ(cached_path, cache_file);

    Node root = parser.parse(html);
    QVBoxLayout layout;
    renderer.render(root, &layout);
    EXPECT_EQ(layout.count(), 1);
    fs::remove(cache_file);
}