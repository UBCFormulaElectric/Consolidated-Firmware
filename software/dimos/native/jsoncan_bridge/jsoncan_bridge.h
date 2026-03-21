#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize jsoncan RX table to startup values.
void dimos_jsoncan_init(void);

// Feed one received CAN frame into jsoncan. `dlc` is clamped to 8 bytes.
void dimos_jsoncan_process_frame(uint32_t std_id, uint32_t dlc, const uint8_t* data);

// Proof-of-integration getter: returns a stable demo u32 from the RX table.
uint32_t dimos_jsoncan_get_demo_u32(void);

#ifdef __cplusplus
}  // extern "C"
#endif
