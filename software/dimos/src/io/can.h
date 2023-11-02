#ifndef QT_DEMO_CAN_H
#define QT_DEMO_CAN_H

#include <optional>
#include "../shared/types.h"

enum CanConnectionError {
	SocketError,
	BindError
};

enum CanReadError {
	ReadInterfaceNotCreated,
	SocketReadError,
	IncompleteCanFrame
};

enum CanWriteError {
	WriteInterfaceNotCreated,
	SocketWriteError,
};

struct TEMP_FAKE_CAN_FRAME {};

Result<std::monostate, CanConnectionError> Can_Init();

Result<TEMP_FAKE_CAN_FRAME, CanReadError> Can_Read();

Result<std::monostate, CanWriteError> Can_Write();

#endif //QT_DEMO_CAN_H
