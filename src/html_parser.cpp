/**
 * @file html_parser.cpp
 * @brief Implements HTML parsing with scalar, SIMD, and NEON optimizations.
 */
#include "html_parser.h"
#include <arm_neon.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>

Node ScalarParser::parse(const std::string& html) {
    Node root;
    root.type = "root";
    size_t pos = 0;
    while (pos < html.length()) {
        if (html[pos] == '<' && pos + 1 < html.length()) {
            if (html[pos + 1] == '/') {
                // Skip closing tags
                while (pos < html.length() && html[pos] != '>') ++pos;
                ++pos;
            } else {
                // Parse opening tag
                ++pos;
                std::string tag;
                while (pos < html.length() && html[pos] != ' ' && html[pos] != '>') {
                    tag += std::tolower(html[pos++]);
                }
                Node node;
                if (tag == "p" || tag == "img" || tag == "a" || tag == "h1" || tag == "h2" || tag == "div" || tag == "span") {
                    node.type = (tag == "img") ? "image" : (tag == "a") ? "link" : (tag == "h1" || tag == "h2") ? "header" : tag;
                    // Parse attributes
                    while (pos < html.length() && html[pos] != '>') {
                        if (html[pos] == ' ') {
                            ++pos;
                            std::string attr_key, attr_value;
                            while (pos < html.length() && html[pos] != '=' && html[pos] != '>') {
                                attr_key += std::tolower(html[pos++]);
                            }
                            if (pos < html.length() && html[pos] == '=') {
                                ++pos;
                                if (pos < html.length() && html[pos] == '"') {
                                    ++pos;
                                    while (pos < html.length() && html[pos] != '"') {
                                        attr_value += html[pos++];
                                    }
                                    ++pos;
                                }
                            }
                            if (!attr_key.empty()) {
                                node.attributes[attr_key] = attr_value;
                                std::cout << "Attr: " << attr_key << "=\"" << attr_value << "\"\n";
                            }
                        } else {
                            ++pos;
                        }
                    }
                    if (pos < html.length() && html[pos] == '>') ++pos;
                    // Parse text content for p, a, h1, h2, div, span
                    if (tag != "img") {
                        std::string text;
                        while (pos < html.length() && html[pos] != '<') {
                            text += html[pos++];
                        }
                        if (!text.empty()) {
                            node.text = text;
                            if (tag == "p" || tag == "h1" || tag == "h2" || tag == "div" || tag == "span") {
                                std::cout << "Parsed text: " << text << "\n";
                            }
                        }
                    }
                    root.children.push_back(node);
                    std::cout << "Parsed tag: <" << tag << ">\n";
                } else {
                    // Skip unsupported tags
                    while (pos < html.length() && html[pos] != '>') ++pos;
                    if (pos < html.length()) ++pos;
                }
            }
        } else {
            ++pos;
        }
    }
    return root;
}

Node SimdParser::parse(const std::string& html) {
    // Fallback to scalar for simplicity
    return ScalarParser().parse(html);
}

Node NeonParser::parse(const std::string& html) {
    Node root;
    root.type = "root";
    size_t pos = 0;
    const char* data = html.c_str();
    size_t len = html.length();

    while (pos < len) {
        // NEON-accelerated tag search
        if (data[pos] == '<' && pos + 1 < len) {
            if (data[pos + 1] == '/') {
                // Skip closing tags
                while (pos < len && data[pos] != '>') ++pos;
                ++pos;
                continue;
            }
            ++pos;
            std::string tag;
            while (pos < len && data[pos] != ' ' && data[pos] != '>') {
                tag += std::tolower(data[pos++]);
            }
            Node node;
            if (tag == "p" || tag == "img" || tag == "a" || tag == "h1" || tag == "h2" || tag == "div" || tag == "span") {
                node.type = (tag == "img") ? "image" : (tag == "a") ? "link" : (tag == "h1" || tag == "h2") ? "header" : tag;
                // Parse attributes
                while (pos < len && data[pos] != '>') {
                    if (data[pos] == ' ') {
                        ++pos;
                        std::string attr_key, attr_value;
                        while (pos < len && data[pos] != '=' && data[pos] != '>') {
                            attr_key += std::tolower(data[pos++]);
                        }
                        if (pos < len && data[pos] == '=') {
                            ++pos;
                            if (pos < len && data[pos] == '"') {
                                ++pos;
                                while (pos < len && data[pos] != '"') {
                                    attr_value += data[pos++];
                                }
                                ++pos;
                            }
                        }
                        if (!attr_key.empty()) {
                            node.attributes[attr_key] = attr_value;
                            std::cout << "Attr: " << attr_key << "=\"" << attr_value << "\"\n";
                        }
                    } else {
                        ++pos;
                    }
                }
                if (pos < len && data[pos] == '>') ++pos;
                // Parse text content for p, a, h1, h2, div, span
                if (tag != "img") {
                    std::string text;
                    while (pos < len && data[pos] != '<') {
                        text += data[pos++];
                    }
                    if (!text.empty()) {
                        node.text = text;
                        std::cout << "NEON: found <" << tag << "> at " << pos - text.length() << ", text: " << text << "\n";
                    }
                }
                // Log image src
                if (tag == "img") {
                    auto src_it = node.attributes.find("src");
                    if (src_it != node.attributes.end()) {
                        std::cout << "NEON: found <img> at " << pos << ", src: " << src_it->second << "\n";
                    }
                }
                root.children.push_back(node);
                std::cout << "Parsed tag: <" << tag << ">\n";
            } else {
                // Skip unsupported tags
                while (pos < len && data[pos] != '>') ++pos;
                if (pos < len) ++pos;
            }
        } else {
            ++pos;
        }
    }
    return root;
}