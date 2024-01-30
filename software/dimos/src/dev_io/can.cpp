#include <iostream>
#include <chrono>
#include <qlogging.h>

#include "can.h"
#include "dev_io_utils.h"

using std::cout, std::endl;

Result<std::monostate, CanConnectionError> Can_Init()
{
    cout << "Can Initialized" << endl;
    return std::monostate{};
}

Result<JsonCanMsg, CanReadError> Can_Read()
{
    wait_delegate_thread();
    qInfo("[dev_CAN] Can Read Requested");
    return JsonCanMsg{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

Result<std::monostate, CanWriteError> Can_Write(const JsonCanMsg *msg)
{
    qInfo("[dev_CAN] Can with id %d Written", msg->std_id);
    return std::monostate{};
}
