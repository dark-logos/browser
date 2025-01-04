/**
 * @file network.h
 * @brief Declaration of the Network class for HTTP requests.
 * @version 1.0
 * @author dark-logos
 */
#ifndef NETWORK_H
#define NETWORK_H

#include <string>

class Network {
public:
  /**
   * Fetches the content of a web page from the given URL.
   * @param url The URL of the web page to fetch.
   * @return The HTML content as a string.
   */
  std::string fetch(const std::string& url);

private:
  /**
   * Callback function for libcurl to handle received data.
   * @param contents Pointer to the received data.
   * @param size Size of each data element.
   * @param nmemb Number of data elements.
   * @param data Pointer to the string to store the data.
   * @return The total size of processed data.
   */
  static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* data);
};

#endif  