#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Reset jsoncan data capture (log/telem) timers.
void dimos_jsoncan_init(void);

// Feed one received CAN frame into the jsoncan RX table. `dlc` is the payload
// length in bytes and is clamped to 64 (CAN FD).
void dimos_jsoncan_process_frame(uint32_t std_id, uint32_t dlc, const uint8_t* data);

// Data capture, generated from each message's `data_capture` config because DIMOS
// is listed in bus.json `loggers`. `time_ms` is a monotonic timestamp. Returns 1
// if the frame should be logged / sent over telem now, else 0.
uint8_t dimos_jsoncan_needs_log(uint32_t std_id, uint32_t time_ms);
uint8_t dimos_jsoncan_needs_telem(uint32_t std_id, uint32_t time_ms);

// Proof-of-integration getter: VC_CanLoggingRemainingErrors from VC_Vitals.
uint8_t dimos_jsoncan_get_vc_can_logging_remaining_errors(void);

#ifdef __cplusplus
}  // extern "C"
#endif
