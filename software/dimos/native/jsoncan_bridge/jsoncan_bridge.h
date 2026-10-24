#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Shutdown loop nodes, in loop order; the indices match the CAN JSON `ShutdownNode` enum.
#define DIMOS_SHUTDOWN_NODE_COUNT 16
// `first_fault_node` when the whole loop is closed (`ShutdownNode::OK`).
#define DIMOS_SHUTDOWN_LOOP_OK 16

// Boards whose `<BOARD>_Vitals` message the dashboard watches, in this order:
// BMS, VC, FSM, RSM, CRIT, DAM.
#define DIMOS_BOARD_COUNT 6
// `board_age_ms` for a board whose vitals have never been received.
#define DIMOS_BOARD_NEVER_SEEN UINT32_MAX

// Everything the dashboard reads from the RX table, copied out in one call.
// lib/data/services/jsoncan_ffi.dart mirrors this layout; change both together.
typedef struct
{
    float    vehicle_velocity;                       // VC_VehicleVelocity (km/h)
    float    ts_power;                               // BMS_TractiveSystemPower (kW, negative is regen)
    float    papps_percent;                          // FSM_PappsMappedPedalPercentage
    uint32_t board_age_ms[DIMOS_BOARD_COUNT];        // Since each board's vitals were last received
    uint16_t board_vitals_id[DIMOS_BOARD_COUNT];     // Each board's vitals message ID
    uint16_t front_brake_pressure;                   // FSM_FrontBrakePressure (psi)
    uint8_t  shutdown_node_ok[DIMOS_SHUTDOWN_NODE_COUNT];
    uint8_t  first_fault_node;                       // VC_FirstFaultNode (ShutdownNode)
    uint8_t  vc_state;                               // VCState
    uint8_t  vc_can_logging_remaining_errors;
    uint8_t  bms_state;                              // BmsState
    uint8_t  bms_soc;                                // BMS_HvBatterySoc (%)
} DimosSignals;

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

// Copy the current RX table values the dashboard uses into `out`. `sizeof(DimosSignals)`
// is returned by dimos_jsoncan_signals_size() so Dart can check its layout matches.
void     dimos_jsoncan_read_signals(DimosSignals* out);
uint32_t dimos_jsoncan_signals_size(void);

#ifdef __cplusplus
}  // extern "C"
#endif
