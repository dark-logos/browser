/**
 * @file network.cpp
 * @brief Implementation of the Network class.
 * @version 1.0
 * @author dark-logos
 */
#include "network.h"
#include <curl/curl.h>

std::string Network::fetch(const std::string& url) {
  CURL* curl = curl_easy_init();
  std::string result;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  return result;
}

size_t Network::writeCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
  data->append((char*)contents, size * nmemb);
  return size * nmemb;
}