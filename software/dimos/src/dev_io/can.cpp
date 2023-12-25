#include "can.h"
#include <iostream>
#include <thread>
#include <chrono>

using std::cout, std::endl;

Result<std::monostate, CanConnectionError> Can_Init()
{
    cout << "Can Initialized" << endl;
    return std::monostate{};
}

Result<CanMsg, CanReadError> Can_Read()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    cout << "Can Read Requested" << endl;
    return CanMsg{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

Result<std::monostate, CanWriteError> Can_Write(const CanMsg *msg)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    cout << "Can with id " << msg->std_id << " Written" << endl;
    return std::monostate{};
}
