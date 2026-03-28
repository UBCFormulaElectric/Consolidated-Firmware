#include "io_rtc.hpp"
#include <stdbool.h>
#include <stdint.h>
#include <span>

#pragma once

void          io_telemRx(void);
void          transmitNTPStartMsg(void);
void          pollForRadioMessages(void);
void          parseNTPPacketBody(std::span<uint8_t> body);
void          tuneRTC(void);
uint64_t      RtcTimeToMs(io::rtc::Time t);
io::rtc::Time MsToRtcTime(uint64_t ms);

void asyncUartTest();
void receiveTest();

typedef struct
{
    uint64_t t0;
    uint64_t t1;
    uint64_t t2;
    uint64_t t3;
} NTPTimestamps;


#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

#include "io_canMsg.hpp"

namespace io::telemMessage::mock
{

inline constexpr uint32_t VC_PUMP_STATUS_CAN_ID = 529U;
inline constexpr uint32_t VC_PUMP_STATUS_DLC    = 2U;

// Packs VC PumpStatus payload:
// - bit 0: PumpFailure (1 bit)
// - bits 1-10: PumpRampUpSetPoint (0.1 % resolution, 0.0-100.0 %)
[[nodiscard]] inline io::CanMsg buildVcPumpStatusCanMsg(const bool pump_failure, const float ramp_up_setpoint_percent)
{
    const float    clamped_percent = std::clamp(ramp_up_setpoint_percent, 0.0F, 100.0F);
    const uint16_t ramp_raw        = static_cast<uint16_t>(clamped_percent * 10.0F + 0.5F);

    std::array<uint8_t, io::CAN_PAYLOAD_BYTES> payload{};
    payload[0] = static_cast<uint8_t>((pump_failure ? 0x01U : 0x00U) | ((ramp_raw & 0x7FU) << 1U));
    payload[1] = static_cast<uint8_t>((ramp_raw >> 7U) & 0x07U);

    return io::CanMsg{
        VC_PUMP_STATUS_CAN_ID,
        VC_PUMP_STATUS_DLC,
        payload,
        false,
        app::can_utils::BusEnum::Bus_FDCAN,
    };
}

} // namespace io::telemMessage::mock