/**
 * @file html_parser.cpp
 * @brief Implements HTML parser and parsing strategies.
 */
#include "html_parser.h"
#include <iostream>
#include <sstream>
#if defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#elif defined(__arm64__)
#include <arm_neon.h>
#endif

// Parse tag attributes (e.g., src="image.jpg")
std::map<std::string, std::string> extractAttributes(const std::string& tag) {
  std::map<std::string, std::string> attributes;
  size_t pos = 1; // skip <
  std::string key, value;

  // Skip tag name
  while (pos < tag.size() && tag[pos] != ' ' && tag[pos] != '>') pos++;
  while (pos < tag.size() && tag[pos] == ' ') pos++;

  // Extract attributes
  while (pos < tag.size() && tag[pos] != '>') {
    key.clear();
    value.clear();

    // Read key
    while (pos < tag.size() && tag[pos] != '=' && tag[pos] != ' ' && tag[pos] != '>') {
      key += tag[pos++];
    }
    while (pos < tag.size() && tag[pos] == ' ') pos++;
    if (pos >= tag.size() || tag[pos] != '=') continue;
    pos++; // skip =
    while (pos < tag.size() && tag[pos] == ' ') pos++;

    // Read value
    bool quoted = false;
    if (pos < tag.size() && tag[pos] == '"') {
      quoted = true;
      pos++;
    }
    while (pos < tag.size() && ((quoted && tag[pos] != '"') || (!quoted && tag[pos] != ' ' && tag[pos] != '>'))) {
      value += tag[pos++];
    }
    if (quoted && pos < tag.size() && tag[pos] == '"') pos++;
    if (!key.empty()) {
      attributes[key] = value;
    }
    while (pos < tag.size() && tag[pos] == ' ') pos++;
  }

  // Debug full tag
  std::cout << "Parsed tag: " << tag << "\n";
  for (const auto& attr : attributes) {
    std::cout << "Attr: " << attr.first << "=\"" << attr.second << "\"\n";
  }
  return attributes;
}

std::string stripHtmlTags(const std::string& text) {
  std::string result;
  bool in_tag = false;
  for (char c : text) {
    if (c == '<') in_tag = true;
    else if (c == '>') in_tag = false;
    else if (!in_tag) result += c;
  }
  return result;
}

Node ScalarParser::parse(const std::string& html) {
  Node root;
  root.type = "root";
  size_t pos = 0;
  while (pos < html.size()) {
    size_t tag_start = html.find('<', pos);
    if (tag_start == std::string::npos) break;

    if (tag_start + 3 < html.size() && html.substr(tag_start, 3) == "<p>") {
      size_t end = html.find("</p>", tag_start);
      if (end == std::string::npos) break;
      Node child;
      child.type = "text";
      child.text = stripHtmlTags(html.substr(tag_start + 3, end - tag_start - 3));
      root.children.push_back(child);
      std::cout << "Scalar: found <p> at " << tag_start << ", text: " << child.text << "\n";
      pos = end + 4;
    } else if (tag_start + 4 < html.size() && html.substr(tag_start, 4) == "<img") {
      size_t tag_end = html.find('>', tag_start);
      if (tag_end == std::string::npos) break;
      Node child;
      child.type = "image";
      child.attributes = extractAttributes(html.substr(tag_start, tag_end - tag_start + 1));
      root.children.push_back(child);
      std::cout << "Scalar: found <img> at " << tag_start << ", src: " << child.attributes["src"] << "\n";
      pos = tag_end + 1;
    } else {
      pos = tag_start + 1;
    }
  }
  return root;
}

#if defined(__x86_64__) || defined(__i386__)
Node SimdParser::parse(const std::string& html) {
  Node root;
  root.type = "root";
  const char* data = html.c_str();
  const size_t len = html.size();
  __m128i tag = _mm_set1_epi8('<');
  size_t pos = 0;

  // Process 16 bytes at a time
  for (; pos + 16 <= len; pos += 16) {
    __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + pos));
    __m128i result = _mm_cmpeq_epi8(chunk, tag);
    int mask = _mm_movemask_epi8(result);

    while (mask != 0) {
      int index = __builtin_ctz(mask);
      size_t tag_pos = pos + index;
      if (tag_pos + 3 < len && data[tag_pos + 1] == 'p' && data[tag_pos + 2] == '>') {
        size_t end = html.find("</p>", tag_pos);
        if (end == std::string::npos) break;
        Node child;
        child.type = "text";
        child.text = stripHtmlTags(html.substr(tag_pos + 3, end - tag_pos - 3));
        root.children.push_back(child);
        std::cout << "SIMD: found <p> at " << tag_pos << ", text: " << child.text << "\n";
      } else if (tag_pos + 4 < len && data[tag_pos + 1] == 'i' && data[tag_pos + 2] == 'm' && data[tag_pos + 3] == 'g') {
        size_t tag_end = html.find('>', tag_pos);
        if (tag_end == std::string::npos) break;
        Node child;
        child.type = "image";
        child.attributes = extractAttributes(html.substr(tag_pos, tag_end - tag_pos + 1));
        root.children.push_back(child);
        std::cout << "SIMD: found <img> at " << tag_pos << ", src: " << child.attributes["src"] << "\n";
      }
      mask &= mask - 1;
    }
  }

  while ((pos = html.find('<', pos)) != std::string::npos) {
    if (pos + 3 < html.size() && html.substr(pos, 3) == "<p>") {
      size_t end = html.find("</p>", pos);
      if (end == std::string::npos) break;
      Node child;
      child.type = "text";
      child.text = stripHtmlTags(html.substr(pos + 3, end - pos - 3));
      root.children.push_back(child);
      std::cout << "Scalar remainder: found <p> at " << pos << ", text: " << child.text << "\n";
      pos = end + 4;
    } else if (pos + 4 < html.size() && html.substr(pos, 4) == "<img") {
      size_t tag_end = html.find('>', pos);
      if (tag_end == std::string::npos) break;
      Node child;
      child.type = "image";
      child.attributes = extractAttributes(html.substr(pos, tag_end - pos + 1));
      root.children.push_back(child);
      std::cout << "Scalar remainder: found <img> at " << pos << ", src: " << child.attributes["src"] << "\n";
      pos = tag_end + 1;
    } else {
      pos++;
    }
  }

  return root;
}
#endif

#if defined(__arm64__)
Node NeonParser::parse(const std::string& html) {
  Node root;
  root.type = "root";
  const char* data = html.c_str();
  const size_t len = html.size();
  uint8x16_t tag = vdupq_n_u8('<');
  size_t pos = 0;

  // Process 16 bytes at a time
  for (; pos + 16 <= len; pos += 16) {
    uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(data + pos));
    uint8x16_t result = vceqq_u8(chunk, tag);
    uint8_t mask[16];
    vst1q_u8(mask, result);

    for (int i = 0; i < 16; ++i) {
      if (mask[i] && pos + i + 4 < len) {
        size_t tag_pos = pos + i;
        if (data[tag_pos + 1] == 'p' && data[tag_pos + 2] == '>') {
          size_t end = html.find("</p>", tag_pos);
          if (end == std::string::npos) continue;
          Node child;
          child.type = "text";
          child.text = stripHtmlTags(html.substr(tag_pos + 3, end - tag_pos - 3));
          root.children.push_back(child);
          std::cout << "NEON: found <p> at " << tag_pos << ", text: " << child.text << "\n";
        } else if (data[tag_pos + 1] == 'i' && data[tag_pos + 2] == 'm' && data[tag_pos + 3] == 'g') {
          size_t tag_end = html.find('>', tag_pos);
          if (tag_end == std::string::npos) continue;
          Node child;
          child.type = "image";
          child.attributes = extractAttributes(html.substr(tag_pos, tag_end - tag_pos + 1));
          root.children.push_back(child);
          std::cout << "NEON: found <img> at " << tag_pos << ", src: " << child.attributes["src"] << "\n";
        }
      }
    }
  }

  while ((pos = html.find('<', pos)) != std::string::npos) {
    if (pos + 3 < html.size() && html.substr(pos, 3) == "<p>") {
      size_t end = html.find("</p>", pos);
      if (end == std::string::npos) break;
      Node child;
      child.type = "text";
      child.text = stripHtmlTags(html.substr(pos + 3, end - pos - 3));
      root.children.push_back(child);
      std::cout << "Scalar remainder: found <p> at " << pos << ", text: " << child.text << "\n";
      pos = end + 4;
    } else if (pos + 4 < html.size() && html.substr(pos, 4) == "<img") {
      size_t tag_end = html.find('>', pos);
      if (tag_end == std::string::npos) break;
      Node child;
      child.type = "image";
      child.attributes = extractAttributes(html.substr(pos, tag_end - pos + 1));
      root.children.push_back(child);
      std::cout << "Scalar remainder: found <img> at " << pos << ", src: " << child.attributes["src"] << "\n";
      pos = tag_end + 1;
    } else {
      pos++;
    }
  }

  return root;
}
#endif