#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>

class Network {
public:
    std::string fetch(const std::string& url) {
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

private:
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
        data->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
};

struct Node {
    std::string tag;
    std::string text;
    std::vector<Node> children;
};

class HtmlParser {
public:
    Node parse(const std::string& html) {
        Node root;
        root.tag = "root";
        size_t pos = 0;
        while ((pos = html.find("<p>", pos)) != std::string::npos) {
            size_t end = html.find("</p>", pos);
            if (end == std::string::npos) break;
            Node child;
            child.tag = "text";
            child.text = html.substr(pos + 3, end - pos - 3);
            root.children.push_back(child);
            pos = end + 4;
        }
        return root;
    }
};

class Renderer {
public:
    void render(const Node& node) {
        if (node.tag == "text") {
            std::cout << node.text << std::endl;
        }
        for (const auto& child : node.children) {
            render(child);
        }
    }
};

int main() {
    Network network;
    std::string html = network.fetch("http://example.com");
    HtmlParser parser;
    Node root = parser.parse(html);
    Renderer renderer;
    renderer.render(root);
    return 0;
}