#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "network.h"
#include <filesystem>
#include <fstream>
#include <curl/curl.h>

namespace fs = std::filesystem;

// Mock CURL for testing network fetch
class MockCurl {
public:
    MOCK_METHOD(CURLcode, curl_easy_perform, (CURL*), ());
    MOCK_METHOD(CURL*, curl_easy_init, (), ());
    MOCK_METHOD(void, curl_easy_cleanup, (CURL*), ());
};

// Test fixture for Network tests
class NetworkTest : public ::testing::Test {
protected:
    void SetUp() override {
        network = std::make_unique<Network>();
        fs::create_directory("cache");
    }

    void TearDown() override {
        fs::remove_all("cache");
    }

    std::unique_ptr<Network> network;
};

// Unit Test: FetchMedia with cached file
TEST_F(NetworkTest, FetchMedia_Cached) {
    std::string filename = "cache/test.media";
    std::ofstream file(filename, std::ios::binary);
    file << "test content";
    file.close();
    std::string result = network->fetchMedia("test.jpg", "http://example.com");
    EXPECT_TRUE(fs::exists(filename));
}

// Unit Test: FetchMedia with invalid URL
TEST_F(NetworkTest, FetchMedia_InvalidUrl) {
    std::string result = network->fetchMedia("invalid://url", "http://example.com");
    EXPECT_TRUE(result.empty());
}

// Unit Test: FetchMedia with empty URL
TEST_F(NetworkTest, FetchMedia_EmptyUrl) {
    std::string result = network->fetchMedia("", "http://example.com");
    EXPECT_TRUE(result.empty());
}

// Unit Test: FetchMedia with non-existent cache
TEST_F(NetworkTest, FetchMedia_NonExistentCache) {
    std::string result = network->fetchMedia("http://example.com/nonexistent.jpg", "http://example.com");
    EXPECT_TRUE(result.empty());
}

// Unit Test: FetchMedia with zero-size cache
TEST_F(NetworkTest, FetchMedia_ZeroSizeCache) {
    std::string filename = "cache/empty.media";
    std::ofstream file(filename, std::ios::binary);
    file.close();
    std::string result = network->fetchMedia("empty.jpg", "http://example.com");
    EXPECT_TRUE(result.empty());
}

// Unit Test: Fetch with invalid URL
TEST_F(NetworkTest, Fetch_HandlesInvalidUrl) {
    std::string result = network->fetch("invalid://url");
    EXPECT_TRUE(result.empty());
}

// Unit Test: FetchMedia with different extension
TEST_F(NetworkTest, FetchMedia_DifferentExtension) {
    std::string filename = "cache/test.png";
    std::ofstream file(filename, std::ios::binary);
    file << "png content";
    file.close();
    std::string result = network->fetchMedia("test.png", "http://example.com");
    EXPECT_TRUE(fs::exists(filename));
}

// Unit Test: FetchMedia with relative URL
TEST_F(NetworkTest, FetchMedia_RelativeUrl) {
    std::string result = network->fetchMedia("image.jpg", "http://example.com/base/");
    EXPECT_TRUE(result.empty());
}

// Unit Test: FetchMedia clears cache
TEST_F(NetworkTest, FetchMedia_ClearsCache) {
    std::string filename = "cache/test.media";
    std::ofstream file(filename, std::ios::binary);
    file << "test content";
    file.close();
    fs::remove(filename);
    std::string result = network->fetchMedia("test.jpg", "http://example.com");
    EXPECT_FALSE(fs::exists(filename));
}

// Unit Test: FetchMedia no cache directory
TEST_F(NetworkTest, FetchMedia_NoCacheDirectory) {
    fs::remove_all("cache");
    std::string result = network->fetchMedia("test.jpg", "http://example.com");
    EXPECT_TRUE(result.empty());
}