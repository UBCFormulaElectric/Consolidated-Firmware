#include "app_leds.h"
#include "io_leds.h"
extern "C"
{
#include "app_canRx.h"
#include "app_canAlerts.h"
}

enum class BoardLEDStatus
{
    OK,
    WARNING,
    FAULT,
    NOT_IMPLEMENTED,
    MISSING_HEARTBEAT
};

static BoardLEDStatus worstBoardStatus(CanAlertBoard board)
{
    bool is_missing_heartbeat;
    switch (board)
    {
        case BMS_ALERT_BOARD:
        {
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get();
            break;
        }
        case CRIT_ALERT_BOARD:
        {
            is_missing_heartbeat = false; // lmao like
            break;
        }
        case FVC_ALERT_BOARD:
        {
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingFVCHeartbeat_get();
            break;
        }
        case RVC_ALERT_BOARD:
        {
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingRVCHeartbeat_get();
            break;
        }
        case RSM_ALERT_BOARD:
        {
            is_missing_heartbeat = app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get();
            break;
        }
        default:
            is_missing_heartbeat = false;
    }
    if (is_missing_heartbeat)
    {
        return BoardLEDStatus::MISSING_HEARTBEAT;
    }
    else if (app_canAlerts_BoardHasFault(board))
    {
        return BoardLEDStatus::FAULT;
    }
    else if (app_canAlerts_BoardHasWarning(board))
    {
        return BoardLEDStatus::WARNING;
    }
    return BoardLEDStatus::OK;
}

io::led::RgbColor board_status_to_rgb(const BoardLEDStatus status)
{
    switch (status)
    {
        case BoardLEDStatus::FAULT:
            return io::led::RgbColor::RED;
        case BoardLEDStatus::OK:
            return io::led::RgbColor::GREEN;
        case BoardLEDStatus::WARNING:
            return io::led::RgbColor::YELLOW;
        case BoardLEDStatus::NOT_IMPLEMENTED:
            return io::led::RgbColor::WHITE;
        case BoardLEDStatus::MISSING_HEARTBEAT:
        default:
            return io::led::RgbColor::OFF;
    }
}

namespace app::leds
{
void update()
{
    io::leds::imd_led.set(app_canRx_BMS_ImdLatchedFault_get());
    io::leds::bspd_led.set(app_canRx_BMS_BspdLatchedFault_get());
    io::leds::ams_led.set(app_canRx_BMS_BmsLatchedFault_get());
    io::leds::start_led.set(app_canRx_RVC_State_get() == RVC_DRIVE_STATE);
    io::leds::regen_led.set(app_canRx_RVC_RegenEnabled_get());
    io::leds::torquevec_led.set(app_canRx_RVC_TorqueVectoringEnabled_get());

    // or driven by BMS_drive_state???
    io::leds::shutdown_status_led.set(board_status_to_rgb(
        app_canRx_RVC_FirstFaultNode_get() == SHDN_OK ? BoardLEDStatus::OK : BoardLEDStatus::FAULT));

    io::leds::bms_status_led.set(board_status_to_rgb(worstBoardStatus(BMS_ALERT_BOARD)));
    io::leds::fvc_status_led.set(board_status_to_rgb(worstBoardStatus(FVC_ALERT_BOARD)));
    io::leds::rvc_status_led.set(board_status_to_rgb(worstBoardStatus(RVC_ALERT_BOARD)));
    io::leds::rsm_status_led.set(board_status_to_rgb(worstBoardStatus(RSM_ALERT_BOARD)));
    io::leds::aux_status_led.set(board_status_to_rgb(BoardLEDStatus::NOT_IMPLEMENTED)); // TODO AUX status
    io::leds::crit_status_led.set(board_status_to_rgb(worstBoardStatus(CRIT_ALERT_BOARD)));
}
} // namespace app::leds
