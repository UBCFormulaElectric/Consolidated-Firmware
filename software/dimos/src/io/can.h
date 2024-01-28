#pragma once
#include "types.h"

extern "C"
{
#include "io_canTx.h"
};

enum CanConnectionError
{
    SocketError,
    BindError
};

enum CanReadError
{
    ReadInterfaceNotCreated,
    SocketReadError,
    IncompleteCanFrame,
    Timeout,
};

enum CanWriteError
{
    WriteInterfaceNotCreated,
    SocketWriteError,
};

Result<std::monostate, CanConnectionError> Can_Init();

Result<JsonCanMsg, CanReadError> Can_Read();

Result<std::monostate, CanWriteError> Can_Write(const JsonCanMsg *msg);