#include <gtest/gtest.h>
#include "network.h"
#include <filesystem>

namespace fs = std::filesystem;

TEST(NetworkTest, ResolveUrlAbsolute) {
    std::string url = "https://example.com/image.jpg";
    std::string base_url = "https://example.com/page.html";
    std::string resolved = resolveUrl(url, base_url);
    EXPECT_EQ(resolved, url);
}

TEST(NetworkTest, ResolveUrlRelative) {
    std::string url = "image.jpg";
    std::string base_url = "https://example.com/page/";
    std::string resolved = resolveUrl(url, base_url);
    EXPECT_EQ(resolved, "https://example.com/page/image.jpg");
}

TEST(NetworkTest, ResolveUrlRootRelative) {
    std::string url = "/image.jpg";
    std::string base_url = "https://example.com/page/subpage/";
    std::string resolved = resolveUrl(url, base_url);
    EXPECT_EQ(resolved, "https://example.com/image.jpg");
}

TEST(NetworkTest, ResolveUrlProtocolRelative) {
    std::string url = "//example.com/image.jpg";
    std::string base_url = "https://example.com/page/";
    std::string resolved = resolveUrl(url, base_url);
    EXPECT_EQ(resolved, "https://example.com/image.jpg");
}

TEST(NetworkTest, ResolveUrlEmpty) {
    std::string url = "";
    std::string base_url = "https://example.com/";
    std::string resolved = resolveUrl(url, base_url);
    EXPECT_EQ(resolved, "");
}

TEST(NetworkTest, FetchMediaCacheHit) {
    Network network;
    fs::create_directory("cache");
    std::string cache_file = "cache/1234567890.media";
    std::ofstream file(cache_file, std::ios::binary);
    file << "fake data";
    file.close();
    std::string result = network.fetchMedia("https://example.com/test.jpg", "https://example.com/");
    EXPECT_EQ(result, cache_file);
    fs::remove(cache_file);
}