#include "StartupPage.h"

StartupPage::StartupPage(QWidget *parent): QFrame(parent), ui(new ui::StartupPage()) {
    ui->setupUi(this);
}

