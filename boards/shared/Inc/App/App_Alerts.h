#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_AlertsConfig.h"

///**
// * Initialize the alerts module.
// */
//void App_Alerts_Init(void);
//
///**
// * Set an aperiodic alert, for logging over CAN. The message will only be sent if the state has changed, i.e.
// * cleared to set, or set to cleared.
// * @param id: The ID of the alert to set/clear
// * @param set: Set alert if true, clear alert if false
// */
//void App_Alerts_SetAlert(BOARD_ALERT id, bool set);
//
///**
// * Set and then immediately clear an aperiodic alert. This is useful for conditions that don't have a defined duration and are only
// * active for an instant, such as board startup. (Similar to a push notification)
// * @param id: The ID of the alert to push
// */
//void App_Alerts_PushAlert(BOARD_ALERT id);
