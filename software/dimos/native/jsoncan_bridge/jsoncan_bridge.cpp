#include "jsoncan_bridge.h"

#include <algorithm>
#include <cstring>

#include "app_canDataCapture.hpp"
#include "app_canRx.hpp"
#include "io_canRx.hpp"

void dimos_jsoncan_init(void)
{
    app::can_data_capture::init();
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
}

uint8_t dimos_jsoncan_needs_log(uint32_t std_id, uint32_t time_ms)
{
    return app::can_data_capture::needsLog(std_id, time_ms) ? 1U : 0U;
}

uint8_t dimos_jsoncan_needs_telem(uint32_t std_id, uint32_t time_ms)
{
    return app::can_data_capture::needsTelem(std_id, time_ms) ? 1U : 0U;
}

uint8_t dimos_jsoncan_get_vc_can_logging_remaining_errors(void)
{
    return app::can_rx::VC_CanLoggingRemainingErrors_get();
}
