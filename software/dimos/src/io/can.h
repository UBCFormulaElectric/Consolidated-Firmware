#pragma once

#include <optional>
#include <cstdint>
#include "types.h"

extern "C"
{
#include "Io_CanTx.h"
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
    IncompleteCanFrame
};

enum CanWriteError
{
    WriteInterfaceNotCreated,
    SocketWriteError,
};

Result<std::monostate, CanConnectionError> Can_Init();

Result<CanMsg, CanReadError> Can_Read();

Result<std::monostate, CanWriteError> Can_Write(const CanMsg *msg);