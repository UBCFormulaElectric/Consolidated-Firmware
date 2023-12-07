#pragma once

#include <QTimer>
#include <QKeyEvent>
#include <QThread>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);

  protected:
    void keyPressEvent(QKeyEvent *event) override;

  private:
    std::unique_ptr<ui::MainWindow> ui;

    void                     setupCanBroadcasting();
    [[noreturn]] static void CanRXTask();
    [[noreturn]] static void CanPeriodicTXTask();
    QTimer                   tx100Hz{};
    QTimer                   tx1Hz{};
    std::unique_ptr<QThread> CanRxTaskThread{};
    std::unique_ptr<QThread> CanTxPeriodicTaskThread{};
};