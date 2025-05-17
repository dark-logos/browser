/**
 * @file html_parser.cpp
 * @brief Implementation of the HtmlParser class.
 * @version 1.0
 * @author dark-logos
 */
#include "html_parser.h"
#include <iostream>
#if defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#elif defined(__arm64__)
#include <arm_neon.h>
#endif

Node ScalarParser::parse(const std::string& html) {
  Node root;
  root.tag = "root";
  // Скалярный парсер: поиск тегов <p>
  size_t pos = 0;
  while ((pos = html.find("<p>", pos)) != std::string::npos) {
    size_t end = html.find("</p>", pos);
    if (end == std::string::npos) break;
    Node child;
    child.tag = "text";
    child.text = html.substr(pos + 3, end - pos - 3);
    root.children.push_back(child);
    std::cout << "Скалярный парсер: найдено <p> на позиции " << pos << ", текст: " << child.text << "\n";
    pos = end + 4;
  }
  return root;
}

#if defined(__x86_64__) || defined(__i386__)
Node SimdParser::parse(const std::string& html) {
  Node root;
  root.tag = "root";
  // SSE4.2-ускоренный поиск тегов <p>
  const char* data = html.c_str();
  const size_t len = html.size();
  __m128i tag = _mm_set1_epi8('<');
  size_t pos = 0;

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
        child.tag = "text";
        child.text = html.substr(tag_pos + 3, end - tag_pos - 3);
        root.children.push_back(child);
        std::cout << "SIMD-парсер: найдено <p> на позиции " << tag_pos << ", текст: " << child.text << "\n";
      }
      mask &= mask - 1;
    }
  }

  // Скалярный парсер для оставшихся байтов
  while ((pos = html.find("<p>", pos)) != std::string::npos) {
    size_t end = html.find("</p>", pos);
    if (end == std::string::npos) break;
    Node child;
    child.tag = "text";
    child.text = html.substr(pos + 3, end - pos - 3);
    root.children.push_back(child);
    std::cout << "Скалярный парсер (остаток): найдено <p> на позиции " << pos << ", текст: " << child.text << "\n";
    pos = end + 4;
  }

  return root;
}
#endif

#if defined(__arm64__)
Node NeonParser::parse(const std::string& html) {
  Node root;
  root.tag = "root";
  // NEON-ускоренный поиск тегов <p>
  const char* data = html.c_str();
  const size_t len = html.size();
  uint8x16_t tag = vdupq_n_u8('<'); // Ищем символ '<'
  size_t pos = 0;

  // Обрабатываем 16 байт за раз с помощью NEON
  for (; pos + 16 <= len; pos += 16) {
    uint8x16_t chunk = vld1q_u8(reinterpret_cast<const uint8_t*>(data + pos));
    uint8x16_t result = vceqq_u8(chunk, tag);
    uint8_t mask[16];
    vst1q_u8(mask, result); // Получаем маску как массив байтов

    // Проверяем, где найдены '<'
    for (int i = 0; i < 16; ++i) {
      if (mask[i] && pos + i + 3 < len && data[pos + i + 1] == 'p' && data[pos + i + 2] == '>') {
        size_t tag_pos = pos + i;
        size_t end = html.find("</p>", tag_pos);
        if (end == std::string::npos) break;
        Node child;
        child.tag = "text";
        child.text = html.substr(tag_pos + 3, end - tag_pos - 3);
        root.children.push_back(child);
        std::cout << "NEON-парсер: найдено <p> на позиции " << tag_pos << ", текст: " << child.text << "\n";
      }
    }
  }

  // Скалярный парсер для оставшихся байтов
  while ((pos = html.find("<p>", pos)) != std::string::npos) {
    size_t end = html.find("</p>", pos);
    if (end == std::string::npos) break;
    Node child;
    child.tag = "text";
    child.text = html.substr(pos + 3, end - pos - 3);
    root.children.push_back(child);
    std::cout << "Скалярный парсер (остаток): найдено <p> на позиции " << pos << ", текст: " << child.text << "\n";
    pos = end + 4;
  }

  return root;
}
#endif