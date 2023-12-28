// ui
#include "mainwindow/MainWindow.h"
// can
#include "can.h"
extern "C"
{
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "Io_CanRx.h"
#include "Io_CanTx.h"
}

constexpr uint32_t GIT_COMMIT_HASH  = 0x00000000;
constexpr bool GIT_COMMIT_CLEAN = false;

void init_can();

int main(int argc, char *argv[])
{
    init_can();

    QApplication a(argc, argv);
    MainWindow   w;
#ifdef USING_dimos
    w.setWindowState(Qt::WindowFullScreen);
#endif
    w.show();
    w.dumpObjectTree();

    return QApplication::exec();
}

void init_can()
{
    // IO init
    Io_CanTx_Init(reinterpret_cast<void (*)(const CanMsg *)>(Can_Write));
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    // clear tables
    App_CanTx_Init();
    App_CanRx_Init();
    // TODO commit hash
    App_CanTx_dimos_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_dimos_Clean_Set(GIT_COMMIT_CLEAN);
}