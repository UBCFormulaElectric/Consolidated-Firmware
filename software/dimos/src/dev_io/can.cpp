#include "can.h"


Result<std::monostate, CanConnectionError> Can_Init() {
	return std::monostate{};
}

Result<TEMP_FAKE_CAN_FRAME, CanReadError> Can_Read() {
	return TEMP_FAKE_CAN_FRAME{};
}

Result<std::monostate, CanWriteError> Can_Write() {
	return std::monostate{};
}