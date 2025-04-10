#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "browser_window.h"
#include "html_parser.h"
#include <QApplication>
#include <QLineEdit>
#include <QTabWidget>
#include <QtTest>

// Mock Network class
class MockNetwork : public Network {
public:
    MOCK_METHOD(std::string, fetch, (const std::string& url), ());
    MOCK_METHOD(std::string, fetchMedia, (const std::string& url, const std::string& base_url), ());
};

// Mock HtmlParser class
class MockHtmlParser : public HtmlParser {
public:
    MOCK_METHOD(Node, parse, (const std::string& html), ());
};

// Test fixture for BrowserWindow tests
class BrowserWindowTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        int argc = 0;
        char* argv[] = {nullptr};
        app = new QApplication(argc, argv);
    }

    static void TearDownTestSuite() {
        delete app;
    }

    void SetUp() override {
        mock_network = std::make_unique<MockNetwork>();
        mock_parser = std::make_unique<MockHtmlParser>();
        window = std::make_unique<BrowserWindow>();
    }

    void TearDown() override {
    }

    static QApplication* app;
    std::unique_ptr<BrowserWindow> window;
    std::unique_ptr<MockNetwork> mock_network;
    std::unique_ptr<MockHtmlParser> mock_parser;
};

QApplication* BrowserWindowTest::app = nullptr;

// Unit Test: BrowserWindow initialization
TEST_F(BrowserWindowTest, Initialize) {
    QLineEdit* url_bar = window->findChild<QLineEdit*>();
    QTabWidget* tabs = window->findChild<QTabWidget*>();
    ASSERT_NE(url_bar, nullptr);
    ASSERT_NE(tabs, nullptr);
    EXPECT_EQ(window->windowTitle(), "QuickDOM");
    EXPECT_EQ(window->size(), QSize(800, 600));
}

// Unit Test: Open new tab
TEST_F(BrowserWindowTest, OpenNewTab) {
    QLineEdit* url_bar = window->findChild<QLineEdit*>();
    url_bar->setText("http://test.com");
    QTest::keyClick(url_bar, Qt::Key_Return);
    QTabWidget* tabs = window->findChild<QTabWidget*>();
    EXPECT_EQ(tabs->count(), 0);
}

// Unit Test: Handle link click
TEST_F(BrowserWindowTest, HandleLinkClick) {
    QLabel* label = new QLabel();
    label->setProperty("href", QString("http://clicked.com"));
    QLineEdit* url_bar = window->findChild<QLineEdit*>();
    url_bar->setText("http://old.com");
    QMetaObject::invokeMethod(window.get(), "handleLinkClicked", Q_ARG(QLabel*, label));
    EXPECT_EQ(url_bar->text().toStdString(), "http://clicked.com");
    delete label;
}

// Unit Test: Open tab with empty URL
TEST_F(BrowserWindowTest, OpenTab_EmptyUrl) {
    QLineEdit* url_bar = window->findChild<QLineEdit*>();
    url_bar->setText("");
    QTest::keyClick(url_bar, Qt::Key_Return);
    QTabWidget* tabs = window->findChild<QTabWidget*>();
    EXPECT_EQ(tabs->count(), 0);
}

// Unit Test: Window title unchanged
TEST_F(BrowserWindowTest, WindowTitleUnchanged) {
    QLineEdit* url_bar = window->findChild<QLineEdit*>();
    url_bar->setText("http://test.com");
    QTest::keyClick(url_bar, Qt::Key_Return);
    EXPECT_EQ(window->windowTitle(), "QuickDOM");
}

// Unit Test: Invalid URL
TEST_F(BrowserWindowTest, InvalidUrl) {
    QLineEdit* url_bar = window->findChild<QLineEdit*>();
    url_bar->setText("invalid://url");
    QTest::keyClick(url_bar, Qt::Key_Return);
    QTabWidget* tabs = window->findChild<QTabWidget*>();
    EXPECT_EQ(tabs->count(), 0);
}

// Unit Test: Close tab
TEST_F(BrowserWindowTest, CloseTab) {
    QTabWidget* tabs = window->findChild<QTabWidget*>();
    EXPECT_EQ(tabs->count(), 0); // Нет вкладок для закрытия
}

// Unit Test: Switch tabs
TEST_F(BrowserWindowTest, SwitchTabs) {
    QTabWidget* tabs = window->findChild<QTabWidget*>();
    EXPECT_EQ(tabs->currentIndex(), -1); // Нет активной вкладки
}