/**
 * @file network.cpp
 * @brief Implements network module for fetching content.
 */
#include "network.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>  // For cout, cerr
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

// Callback for libcurl data
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  auto* str = static_cast<std::string*>(userp);
  str->append(static_cast<char*>(contents), size * nmemb);
  return size * nmemb;
}

std::string Network::fetch(const std::string& url) {
  CURL* curl = curl_easy_init();
  std::string response;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "Curl error: " << curl_easy_strerror(res) << "\n";
    }
    curl_easy_cleanup(curl);
  }
  return response;
}

std::string Network::fetchMedia(const std::string& url) {
  // Generate cache filename from URL hash
  std::hash<std::string> hasher;
  std::string filename = "cache/" + std::to_string(hasher(url)) + ".media";

  if (fs::exists(filename)) {
    std::cout << "Using cached media: " << filename << "\n";
    return filename;
  }

  fs::create_directory("cache");

  CURL* curl = curl_easy_init();
  std::ofstream file(filename, std::ios::binary);
  if (curl && file.is_open()) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "Media fetch error: " << curl_easy_strerror(res) << "\n";
      filename.clear();
    }
    curl_easy_cleanup(curl);
    file.close();
  }
  return filename;
}