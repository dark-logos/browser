/**
 * @file network.h
 * @brief Defines network module for fetching web content and media.
 */
#ifndef NETWORK_H
#define NETWORK_H

#include <string>

/**
 * @class Network
 * @brief Fetches web pages and media files.
 */
class Network {
public:
  /**
   * @brief Fetches HTML content from a URL.
   * @param url Web page URL.
   * @return HTML content as a string.
   */
  std::string fetch(const std::string& url);

  /**
   * @brief Fetches and caches a media file.
   * @param url Media file URL.
   * @return Path to the cached file.
   */
  std::string fetchMedia(const std::string& url);
};

#endif