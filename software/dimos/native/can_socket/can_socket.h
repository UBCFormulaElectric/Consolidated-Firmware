#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Max CAN FD payload length in bytes.
#define DIMOS_CAN_MAX_PAYLOAD 64

// Opens a raw SocketCAN socket on `ifname` (e.g. "can0", "vcan0") that receives both
// classic and CAN FD frames. Returns the socket fd (>= 0), or a negative errno:
// -ENODEV if the interface doesn't exist, -ENETDOWN if it isn't up.
// Linux only; returns -ENOTSUP elsewhere.
int32_t dimos_can_open(const char* ifname);

// Waits up to `timeout_ms` for one data frame. Returns 1 and fills `can_id`, `len`
// (0-64) and `data` (DIMOS_CAN_MAX_PAYLOAD bytes) when a frame was read, 0 on timeout
// (or a non-data frame), or a negative errno on error.
int32_t dimos_can_read(int32_t fd, int32_t timeout_ms, uint32_t* can_id, uint8_t* len, uint8_t* data);

void dimos_can_close(int32_t fd);

#ifdef __cplusplus
}  // extern "C"
#endif
