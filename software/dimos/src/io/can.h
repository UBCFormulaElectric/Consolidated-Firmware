#pragma once
#include "types.h"

extern "C"
{
#include "io_canTx.h"
};

enum class CanConnectionError
{
    SocketError,
    BindError
};

enum class CanReadError
{
    ReadInterfaceNotCreated,
    SocketReadError,
    IncompleteCanFrame,
    Timeout,
};

enum class CanWriteError
{
    WriteInterfaceNotCreated,
    SocketWriteError
};

Result<std::monostate, CanConnectionError> Can_Init();

Result<JsonCanMsg, CanReadError> Can_Read();

Result<std::monostate, CanWriteError> Can_Write(const JsonCanMsg *msg);