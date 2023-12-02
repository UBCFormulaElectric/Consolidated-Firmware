#pragma once

#include <QWidget>

class Squircle : public QWidget {
public:
    explicit Squircle(int radius, QWidget *parent = nullptr);
    void setRadius(int radius);
    void setupUi();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int radius = 1;
};

