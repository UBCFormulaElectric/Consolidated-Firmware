#include "StartupPage.h"

StartupPage::StartupPage(QWidget *parent) : QFrame(parent)
{
    this->setGeometry(QRect(0, 0, 1024, 600));
    ui->setupUi(this);
}
