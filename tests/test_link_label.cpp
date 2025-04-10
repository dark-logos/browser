#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "link_label.h"
#include "browser_window.h"
#include <QApplication>
#include <QLineEdit>
#include <QtTest>

// Test fixture for LinkLabel tests
class LinkLabelTest : public ::testing::Test {
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
        link_label = new LinkLabel();
    }

    void TearDown() override {
        delete link_label;
    }

    static QApplication* app;
    LinkLabel* link_label;
};

QApplication* LinkLabelTest::app = nullptr;

// Unit Test: Emit clicked signal on left click
TEST_F(LinkLabelTest, EmitClickedOnLeftClick) {
    bool clicked = false;
    QObject::connect(link_label, &LinkLabel::clicked, [&](QLabel* label) {
        clicked = true;
        EXPECT_EQ(label, link_label);
    });
    QTest::mouseClick(link_label, Qt::LeftButton, Qt::NoModifier, QPoint(10, 10));
    EXPECT_TRUE(clicked);
}

// Unit Test: No emit clicked on right click
TEST_F(LinkLabelTest, NoEmitClickedOnRightClick) {
    bool clicked = false;
    QObject::connect(link_label, &LinkLabel::clicked, [&](QLabel*) {
        clicked = true;
    });
    QTest::mouseClick(link_label, Qt::RightButton, Qt::NoModifier, QPoint(10, 10));
    EXPECT_FALSE(clicked);
}

// Unit Test: Set text
TEST_F(LinkLabelTest, SetText) {
    link_label->setText("Test Link");
    EXPECT_EQ(link_label->text().toStdString(), "Test Link");
}

// Unit Test: Set stylesheet
TEST_F(LinkLabelTest, SetStyleSheet) {
    link_label->setStyleSheet("color: blue;");
    EXPECT_EQ(link_label->styleSheet(), "color: blue;");
}

// Unit Test: Set property
TEST_F(LinkLabelTest, SetProperty) {
    link_label->setProperty("href", QString("http://test.com"));
    EXPECT_EQ(link_label->property("href").toString().toStdString(), "http://test.com");
}

// Unit Test: Browser interaction
TEST_F(LinkLabelTest, BrowserInteraction) {
    BrowserWindow window;
    link_label->setProperty("href", QString("http://test.com"));
    link_label->setText("Click me");
    bool clicked = false;
    QObject::connect(link_label, &LinkLabel::clicked, [&](QLabel* label) {
        clicked = true;
        QMetaObject::invokeMethod(&window, "handleLinkClicked", Q_ARG(QLabel*, label));
    });
    QTest::mouseClick(link_label, Qt::LeftButton, Qt::NoModifier, QPoint(10, 10));
    EXPECT_TRUE(clicked);
    QLineEdit* url_bar = window.findChild<QLineEdit*>();
    ASSERT_NE(url_bar, nullptr);
    EXPECT_EQ(url_bar->text().toStdString(), "http://test.com");
}

// Unit Test: No href
TEST_F(LinkLabelTest, NoHref) {
    bool clicked = false;
    QObject::connect(link_label, &LinkLabel::clicked, [&](QLabel*) {
        clicked = true;
    });
    QTest::mouseClick(link_label, Qt::LeftButton, Qt::NoModifier, QPoint(10, 10));
    EXPECT_TRUE(clicked);
}

// Unit Test: Empty text
TEST_F(LinkLabelTest, EmptyText) {
    link_label->setText("");
    EXPECT_EQ(link_label->text().toStdString(), "");
    bool clicked = false;
    QObject::connect(link_label, &LinkLabel::clicked, [&](QLabel*) {
        clicked = true;
    });
    QTest::mouseClick(link_label, Qt::LeftButton, Qt::NoModifier, QPoint(10, 10));
    EXPECT_TRUE(clicked);
}

// Unit Test: Multiple clicks
TEST_F(LinkLabelTest, MultipleClicks) {
    int click_count = 0;
    QObject::connect(link_label, &LinkLabel::clicked, [&](QLabel*) {
        ++click_count;
    });
    QTest::mouseClick(link_label, Qt::LeftButton, Qt::NoModifier, QPoint(10, 10));
    QTest::mouseClick(link_label, Qt::LeftButton, Qt::NoModifier, QPoint(20, 20));
    EXPECT_EQ(click_count, 2);
}

// Unit Test: Cursor shape
TEST_F(LinkLabelTest, CursorShape) {
    link_label->setCursor(Qt::PointingHandCursor);
    EXPECT_EQ(link_label->cursor().shape(), Qt::PointingHandCursor);
}

// Unit Test: Different mouse buttons
TEST_F(LinkLabelTest, DifferentMouseButtons) {
    int click_count = 0;
    QObject::connect(link_label, &LinkLabel::clicked, [&](QLabel*) {
        ++click_count;
    });
    QTest::mouseClick(link_label, Qt::LeftButton, Qt::NoModifier, QPoint(10, 10));
    QTest::mouseClick(link_label, Qt::RightButton, Qt::NoModifier, QPoint(10, 10));
    QTest::mouseClick(link_label, Qt::MiddleButton, Qt::NoModifier, QPoint(10, 10));
    EXPECT_EQ(click_count, 1);
}

// Unit Test: Long text
TEST_F(LinkLabelTest, LongText) {
    std::string long_text(1000, 'a');
    link_label->setText(QString::fromStdString(long_text));
    EXPECT_EQ(link_label->text().toStdString(), long_text);
}

// Unit Test: Click with modifier
TEST_F(LinkLabelTest, ClickWithModifier) {
    bool clicked = false;
    QObject::connect(link_label, &LinkLabel::clicked, [&](QLabel*) {
        clicked = true;
    });
    QTest::mouseClick(link_label, Qt::LeftButton, Qt::ControlModifier, QPoint(10, 10));
    EXPECT_TRUE(clicked);
}