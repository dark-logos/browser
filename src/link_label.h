/**
 * @file link_label.h
 * @brief Defines a clickable link label for the browser.
 */
#ifndef LINK_LABEL_H
#define LINK_LABEL_H

#include <QLabel>
#include <QMouseEvent>

/**
 * @class LinkLabel
 * @brief Custom QLabel for handling link interactions.
 */
class LinkLabel : public QLabel {
    Q_OBJECT
public:
    explicit LinkLabel(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void clicked(QLabel* label);
};

#endif // LINK_LABEL_H