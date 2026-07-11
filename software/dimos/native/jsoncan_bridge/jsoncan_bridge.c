#include "jsoncan_bridge.h"

#include <string.h>

#include "app/app_canRx.h"
#include "io/io_canRx.h"

void dimos_jsoncan_init(void)
{
    app_canRx_init();
}

void dimos_jsoncan_process_frame(uint32_t std_id, uint32_t dlc, const uint8_t* data)
{
    JsonCanMsg msg;
    msg.std_id = std_id;

    if (dlc > 8U)
    {
        dlc = 8U;
    }
    msg.dlc = dlc;

    memset(msg.data, 0, sizeof(msg.data));
    if (data != NULL && dlc > 0U)
    {
        memcpy(msg.data, data, dlc);
    }

    io_canRx_updateRxTableWithMessage(&msg);
}

uint32_t dimos_jsoncan_get_demo_u32(void)
{
    // Present on both hexray and quintuna configs, so the demo stays buildable
    // when switching CAN databases.
    return app_canRx_VC_CanLoggingRemainingErrors_get();
}

uint8_t dimos_jsoncan_get_vc_pump_failure_u8(void)
{
    return app_canRx_VC_PumpFailure_get() ? 1U : 0U;
}
