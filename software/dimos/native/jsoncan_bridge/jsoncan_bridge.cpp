#include "jsoncan_bridge.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <optional>

#include "app_canDataCapture.hpp"
#include "app_canRx.hpp"
#include "io_canRx.hpp"

using namespace app::can_utils;
using namespace app::can_rx;

static_assert(static_cast<int>(ShutdownNode::OK) == DIMOS_SHUTDOWN_LOOP_OK);
static_assert(DIMOS_SHUTDOWN_LOOP_OK == DIMOS_SHUTDOWN_NODE_COUNT);

namespace
{
using Clock = std::chrono::steady_clock;

// Same order as DIMOS_BOARD_COUNT documents.
constexpr std::array<uint32_t, DIMOS_BOARD_COUNT> board_vitals_ids = {
    BMS_Vitals_Signals::MSG_ID, VC_Vitals_Signals::MSG_ID,  CRIT_Vitals_Signals::MSG_ID,
    FSM_Vitals_Signals::MSG_ID, RSM_Vitals_Signals::MSG_ID, DAM_Vitals_Signals::MSG_ID,
};

std::array<std::optional<Clock::time_point>, DIMOS_BOARD_COUNT> board_last_seen{};

void recordVitals(uint32_t std_id)
{
    const auto it = std::find(board_vitals_ids.begin(), board_vitals_ids.end(), std_id);
    if (it != board_vitals_ids.end())
    {
        board_last_seen[static_cast<size_t>(it - board_vitals_ids.begin())] = Clock::now();
    }
}

uint32_t ageMs(const std::optional<Clock::time_point>& last_seen, Clock::time_point now)
{
    if (!last_seen.has_value())
    {
        return DIMOS_BOARD_NEVER_SEEN;
    }
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - *last_seen).count();
    return static_cast<uint32_t>(std::min<long long>(ms, DIMOS_BOARD_NEVER_SEEN - 1));
}
} // namespace

void dimos_jsoncan_init(void)
{
    app::can_data_capture::init();
    board_last_seen.fill(std::nullopt);
}

void dimos_jsoncan_process_frame(uint32_t std_id, uint32_t dlc, const uint8_t* data)
{
    JsonCanMsg::data_array payload{};
    dlc = std::min<uint32_t>(dlc, payload.size());
    if (data != nullptr && dlc > 0U)
    {
        std::memcpy(payload.data(), data, dlc);
    }

    io::can_rx::updateRxTableWithMessage(JsonCanMsg(std_id, dlc, payload));
    recordVitals(std_id);
}

uint8_t dimos_jsoncan_needs_log(uint32_t std_id, uint32_t time_ms)
{
    return app::can_data_capture::needsLog(std_id, time_ms) ? 1U : 0U;
}

uint8_t dimos_jsoncan_needs_telem(uint32_t std_id, uint32_t time_ms)
{
    return app::can_data_capture::needsTelem(std_id, time_ms) ? 1U : 0U;
}

void dimos_jsoncan_read_signals(DimosSignals* out)
{
    if (out == nullptr)
    {
        return;
    }
    DimosSignals s{};

    s.vehicle_velocity = static_cast<float>(VC_VehicleVelocity_get());
    s.ts_power         = BMS_TractiveSystemPower_get();
    s.papps_percent    = FSM_PappsMappedPedalPercentage_get();

    const auto now = Clock::now();
    for (size_t i = 0; i < DIMOS_BOARD_COUNT; i++)
    {
        s.board_age_ms[i]    = ageMs(board_last_seen[i], now);
        s.board_vitals_id[i] = static_cast<uint16_t>(board_vitals_ids[i]);
    }
    s.front_brake_pressure = FSM_FrontBrakePressure_get();

    // Indexed by ShutdownNode, i.e. in loop order.
    const std::array<bool, DIMOS_SHUTDOWN_NODE_COUNT> node_ok = {
        BMS_BmsLatchOk_get(),           BMS_ImdLatchOk_get(),           BMS_BspdLatchOk_get(),
        BMS_HVPShdnOKStatus_get(),      BMS_HVNShdnOKStatus_get(),      VC_MSDOrEMeterOKStatus_get(),
        RSM_RearLeftMotorInterlock_get(), VC_RearRightMotorInterlock_get(), DAM_REStopOKStatus_get(),
        DAM_LEStopOKStatus_get(),       FSM_COCKPITOKStatus_get(),      FSM_BOTSOKStatus_get(),
        FSM_FrontRightILCKOKStatus_get(), FSM_FrontLeftILCKOKStatus_get(), VC_InertiaSwitch_get(),
        VC_TSMSOKStatus_get(),
    };
    for (size_t i = 0; i < DIMOS_SHUTDOWN_NODE_COUNT; i++)
    {
        s.shutdown_node_ok[i] = node_ok[i] ? 1U : 0U;
    }
    s.first_fault_node = static_cast<uint8_t>(VC_FirstFaultNode_get());

    s.vc_state                        = static_cast<uint8_t>(VC_State_get());
    s.vc_can_logging_remaining_errors = VC_CanLoggingRemainingErrors_get();
    s.bms_state                       = static_cast<uint8_t>(BMS_State_get());
    s.bms_soc                         = BMS_HvBatterySoc_get();

    *out = s;
}

uint32_t dimos_jsoncan_signals_size(void)
{
    return sizeof(DimosSignals);
}
