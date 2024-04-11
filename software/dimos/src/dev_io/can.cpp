#include <iostream>
#include <chrono>
#include <qlogging.h>

#include "can.h"
#include "dev_io_utils.h"

using std::cout, std::endl;

Result<std::monostate, CanConnectionError> Can_Init()
{
    qInfo("Can Initialized");
    return std::monostate{};
}

Result<JsonCanMsg, CanReadError> Can_Read()
{
    if (const WaitDelegateResult res = wait_delegate_thread(); res == WaitDelegateResult::INTERRUPTED)
        return CanReadError::Timeout;
    //    qInfo("Can Read Requested");
    return JsonCanMsg{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

Result<std::monostate, CanWriteError> Can_Write(const JsonCanMsg *msg)
{
    //    qInfo("Can with id %d Written", msg->std_id);
    return std::monostate{};
}
