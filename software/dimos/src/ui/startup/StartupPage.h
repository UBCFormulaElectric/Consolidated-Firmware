#pragma once

#include <QFrame>
#include "ui_StartupPage.h"

class StartupPage: public QFrame {
    Q_OBJECT
public:
    explicit StartupPage(QWidget *parent = nullptr);

private:
    std::unique_ptr<ui::StartupPage> ui = std::make_unique<ui::StartupPage>();
};