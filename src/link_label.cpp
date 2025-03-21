/**
 * @file link_label.cpp
 * @brief Implements clickable link label.
 */
#include "link_label.h"

LinkLabel::LinkLabel(QWidget* parent) : QLabel(parent) {}

void LinkLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(this);
    }
    QLabel::mousePressEvent(event);
}