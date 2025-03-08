/**
 * @file network.cpp
 * @brief Implements network module for fetching content.
 */
#include "network.h"
#include <curl/curl.h>
#include <fstream>
#include <functional>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// Callback for libcurl data
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  auto* str = static_cast<std::string*>(userp);
  str->append(static_cast<char*>(contents), size * nmemb);
  return size * nmemb;
}

// Callback for writing media to file
size_t writeFileCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  auto* file = static_cast<std::ofstream*>(userp);
  size_t total = size * nmemb;
  file->write(static_cast<char*>(contents), total);
  return total;
}

// Resolve relative URL to absolute
std::string resolveUrl(const std::string& url, const std::string& base_url) {
  if (url.empty()) return "";
  if (url.find("http://") == 0 || url.find("https://") == 0) return url;
  if (url.find("//") == 0) return "https:" + url;

  std::string base = base_url;
  if (base.back() != '/') base += '/';
  if (url.front() == '/') return base.substr(0, base.find('/', 8)) + url;
  return base + url;
}

std::string Network::fetch(const std::string& url) {
  CURL* curl = curl_easy_init();
  std::string response;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Disable SSL verification (temporary)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "Fetch error: " << curl_easy_strerror(res) << " for " << url << "\n";
    }
    curl_easy_cleanup(curl);
  } else {
    std::cerr << "Failed to init curl for " << url << "\n";
  }
  return response;
}

std::string Network::fetchMedia(const std::string& url, const std::string& base_url) {
  std::string resolved_url = resolveUrl(url, base_url);
  if (resolved_url.empty()) {
    std::cerr << "Invalid media URL: " << url << "\n";
    return "";
  }

  // Generate cache filename from URL hash
  std::hash<std::string> hasher;
  std::string filename = "cache/" + std::to_string(hasher(resolved_url)) + ".media";

  if (fs::exists(filename) && fs::file_size(filename) > 0) {
    std::cout << "Using cached media: " << filename << "\n";
    return filename;
  }

  fs::create_directory("cache");

  CURL* curl = curl_easy_init();
  std::ofstream file(filename, std::ios::binary);
  long http_code = 0;
  if (curl && file.is_open()) {
    curl_easy_setopt(curl, CURLOPT_URL, resolved_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Disable SSL verification (temporary)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "QuickDOM/1.0"); // Add User-Agent
    CURLcode res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    file.close();
    if (res != CURLE_OK) {
      std::cerr << "Media fetch error: " << curl_easy_strerror(res) << " for " << resolved_url << "\n";
      fs::remove(filename);
      filename.clear();
    } else if (http_code != 200) {
      std::cerr << "HTTP error: " << http_code << " for " << resolved_url << "\n";
      fs::remove(filename);
      filename.clear();
    } else if (fs::file_size(filename) == 0) {
      std::cerr << "Empty media file: " << filename << "\n";
      fs::remove(filename);
      filename.clear();
    } else {
      std::cout << "Downloaded media: " << filename << " (" << fs::file_size(filename) << " bytes)\n";
    }
    curl_easy_cleanup(curl);
  } else {
    std::cerr << "Failed to init curl or open file for " << resolved_url << "\n";
    if (curl) curl_easy_cleanup(curl);
    if (file.is_open()) file.close();
    filename.clear();
  }

  return filename;
}