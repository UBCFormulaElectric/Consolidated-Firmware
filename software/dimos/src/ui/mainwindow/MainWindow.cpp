#include "MainWindow.h"
#include "io_handlers/can_handlers.h"
#include "io_handlers/gpio_handlers.h"

#include <iostream>
// libraries
extern "C"
{
#include <Io_CanTx.h>
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new ui::MainWindow)
{
    if (const Result<std::monostate, CAN_setup_errors> r_can = setupCanBroadcasting(); r_can.index() == 1)
    {
        can_setup_error = get<CAN_setup_errors>(r_can);
    }
    if (const Result<std::monostate, GPIO_setup_errors> r_gpio = setupGPIO(); r_gpio.index() == 1)
    {
        gpio_setup_error = get<GPIO_setup_errors>(r_gpio);
    }
    ui->setupUi(this);
}

// Testing purposes (GPIO dummy)
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Space:
            ui->toggleSwitcher();
            break;
        case Qt::Key_Return:
            if (ui->getSwitcherOpen())
            {
                ui->toggleSwitcher();
                ui->setSwitcherSelectionToFrame();
                break;
            }
            break;
        case Qt::Key_Right:
            ui->switcherRight();
            break;
        case Qt::Key_Left:
            ui->switcherLeft();
            break;
        default:
            qWarning() << QString::fromStdString("UNHANDLED KEY " + event->text().toStdString());
            break;
    }
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    std::cout << "Closing MainWindow" << std::endl;
    if (CanRxTaskThread) CanRxTaskThread->requestInterruption();
    std::cout << "canrx task requested end" << std::endl;
    if (CanTxPeriodicTaskThread) CanTxPeriodicTaskThread->requestInterruption();
    std::cout << "canperiodictx task requested end" << std::endl;

    for (int i = 0; i < GPIO_COUNT; i++)
    {
        if (gpio_monitor_threads[i].has_value())
            gpio_monitor_threads[i].value()->requestInterruption();
    }
    QMainWindow::closeEvent(event);
}

Result<std::monostate, MainWindow::CAN_setup_errors> MainWindow::setupCanBroadcasting()
{
    // CANTX TASK
    tx100Hz.setInterval(can_handlers::TASK_INTERVAL_100HZ);
    tx100Hz.setSingleShot(false);
    connect(&tx100Hz, &QTimer::timeout, Io_CanTx_Enqueue100HzMsgs);
    tx100Hz.start();

    tx1Hz.setInterval(can_handlers::TASK_INTERVAL_1HZ);
    tx1Hz.setSingleShot(false);
    connect(&tx1Hz, &QTimer::timeout, Io_CanTx_Enqueue1HzMsgs);
    tx1Hz.start();

    CanRxTaskThread         = std::unique_ptr<QThread>(QThread::create(&can_handlers::CanRXTask));
    CanTxPeriodicTaskThread = std::unique_ptr<QThread>(QThread::create(&can_handlers::CanPeriodicTXTask));
    CanRxTaskThread->start();
    CanTxPeriodicTaskThread->start();

    return std::monostate{};
}

Result<std::monostate, MainWindow::GPIO_setup_errors> MainWindow::setupGPIO()
{
    const std::array<bool, GPIO_COUNT> gpio_has_err = gpio_init();
    bool                               has_gpio_err = false;
    for (int i = 0; i < GPIO_COUNT; i++)
    {
        const auto GPIO_ENUM = static_cast<gpio_input>(i);
        if (gpio_has_err[i])
        {
            has_gpio_err = true;
            continue;
        }
        gpio_monitor_threads[i] = std::make_unique<QThread>(QThread::create(&gpio_handlers::gpio_monitor, GPIO_ENUM));
        gpio_monitor_threads[i].value()->start();
    }
    if (has_gpio_err)
        return LINE_SETUP_ERROR;
    return std::monostate{};
}