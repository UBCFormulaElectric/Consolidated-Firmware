#include "can.h"


Result<std::monostate, CanConnectionError> Can_Init() {
	return std::monostate{};
}

Result<CanMsg, CanReadError> Can_Read() {
	return CanMsg{};
}

Result<std::monostate, CanWriteError> Can_Write(const CanMsg *msg) {
	return std::monostate{};
}