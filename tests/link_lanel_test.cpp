#include <gtest/gtest.h>
#include "link_label.h"
#include <QApplication>
#include <QMouseEvent>

TEST(LinkLabelTest, MousePressEmitsClicked) {
    int argc = 0;
    QApplication app(argc, nullptr);
    LinkLabel label;
    bool clicked = false;
    QObject::connect(&label, &LinkLabel::clicked, [&clicked](QLabel*) { clicked = true; });
    QMouseEvent event(QEvent::MouseButtonPress, QPointF(10, 10), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    label.mousePressEvent(&event);
    EXPECT_TRUE(clicked);
}

TEST(LinkLabelTest, NonLeftClickDoesNotEmit) {
    int argc = 0;
    QApplication app(argc, nullptr);
    LinkLabel label;
    bool clicked = false;
    QObject::connect(&label, &LinkLabel::clicked, [&clicked](QLabel*) { clicked = true; });
    QMouseEvent event(QEvent::MouseButtonPress, QPointF(10, 10), Qt::RightButton, Qt::RightButton, Qt::NoModifier);
    label.mousePressEvent(&event);
    EXPECT_FALSE(clicked);
}