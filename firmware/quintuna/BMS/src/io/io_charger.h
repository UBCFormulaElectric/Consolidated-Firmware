#pragma once

typedef enum
{
    EVSE_DISCONNECTED,
    EVSE_CONNECTED,
    EVSE_CHARGING,
    EVSE_ERROR
} EvseStatus;

EvseStatus io_charger_getStatus(void);

void io_charger_pwm_callback(TIM_HandleTypeDef *htim);
