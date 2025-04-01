#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "browser_window.h"
#include "html_parser.h"
#include "network.h"
#include <QApplication>
#include <QVBoxLayout>

using ::testing::Return;
using ::testing::_;

class MockNetwork : public Network {
public:
    MOCK_METHOD(std::string, fetch, (const std::string&), (override));
    MOCK_METHOD(std::string, fetchMedia, (const std::string&, const std::string&), (override));
};

class MockHtmlParser : public HtmlParser {
public:
    MOCK_METHOD(Node, parse, (const std::string&), (override));
};

TEST(BrowserWindowTest, OpenNewTab) {
    int argc = 0;
    QApplication app(argc, nullptr);
    BrowserWindow window;
    window.openNewTab();
    EXPECT_EQ(window.findChild<QTabWidget*>()->count(), 1);
    EXPECT_EQ(window.findChild<QTabWidget*>()->tabText(0).toStdString(), "New Tab");
}

TEST(BrowserWindowTest, HandleLinkClicked) {
    int argc = 0;
    QApplication app(argc, nullptr);
    MockNetwork mock_network;
    MockHtmlParser mock_parser;
    BrowserWindow window;
    window.parser_ = std::make_unique<MockHtmlParser>();

    Node link_node;
    link_node.type = "link";
    link_node.text = "Click me";
    link_node.attributes["href"] = "https://example.com";

    Node new_page_node;
    new_page_node.type = "p";
    new_page_node.text = "New page content";

    EXPECT_CALL(dynamic_cast<MockHtmlParser&>(*window.parser_), parse("<p>New page content</p>"))
        .WillOnce(Return(new_page_node));
    EXPECT_CALL(mock_network, fetch("https://example.com"))
        .WillOnce(Return("<p>New page content</p>"));

    QLabel label;
    label.setProperty("isLink", true);
    label.setProperty("href", QString("https://example.com"));
    window.handleLinkClicked(&label);

    EXPECT_EQ(window.findChild<QLineEdit*>()->text().toStdString(), "https://example.com");
    EXPECT_EQ(window.findChild<QTabWidget*>()->count(), 1);
    EXPECT_EQ(window.findChild<QTabWidget*>()->tabText(0).toStdString(), "https://example.com");
}

TEST(BrowserWindowTest, FreezeAndUnfreezeTab) {
    int argc = 0;
    QApplication app(argc, nullptr);
    BrowserWindow window;
    window.openNewTab();
    window.freezeTab(0);
    EXPECT_TRUE(window.frozen_tabs_.contains(0));
    window.unfreezeTab(0);
    EXPECT_FALSE(window.frozen_tabs_.contains(0));
}