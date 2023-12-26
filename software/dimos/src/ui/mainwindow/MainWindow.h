#pragma once

#include <QTimer>
#include <QKeyEvent>
#include <QThread>

#include "gpio.h"
#include "types.h"
#include "ui_MainWindow.h"

class MainWindow final : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);

  protected:
    void keyPressEvent(QKeyEvent *event) override;

  private:
    std::unique_ptr<ui::MainWindow> ui;

    enum GPIO_setup_errors
    {
        LINE_SETUP_ERROR
    };
    Result<std::monostate, GPIO_setup_errors>                       setupGPIO();
    std::array<std::optional<std::unique_ptr<QThread>>, GPIO_COUNT> gpio_monitor_threads = { std::nullopt };

    enum CAN_setup_errors
    {
    };
    Result<std::monostate, CAN_setup_errors> setupCanBroadcasting();
    QTimer                                   tx100Hz{};
    QTimer                                   tx1Hz{};
    std::unique_ptr<QThread>                 CanRxTaskThread;
    std::unique_ptr<QThread>                 CanTxPeriodicTaskThread;

    std::optional<GPIO_setup_errors> gpio_setup_error = std::nullopt;
    std::optional<CAN_setup_errors>  can_setup_error  = std::nullopt;
};