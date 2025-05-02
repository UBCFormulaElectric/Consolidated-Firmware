#pragma once

typedef enum
{
    EVSE_DISCONNECTED,
    EVSE_CONNECTED,
    EVSE_CHARGING,
    EVSE_ERROR
} EVSE_STATUS;

EVSE_STATUS io_charger_getStatus(void);

void io_charger_pwm_callback(TIM_HandleTypeDef *htim);