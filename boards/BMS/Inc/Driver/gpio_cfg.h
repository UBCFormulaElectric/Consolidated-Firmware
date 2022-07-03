#pragma once

#include "main.h"
#include "gpio_shared.h"

static const gpio_s gpio_cfg[NUM_GPIO_USED] =
{
    [CHARGER_EN] =
    {
        .port = BRUSA_PON_GPIO_Port,
        .pin = BRUSA_PON_Pin,
        .is_gpo = true,
    },
    [IMD_OK] =
    {
        .port = IMD_OK_GPIO_Port,
        .pin = IMD_OK_Pin,
        .is_gpo = false,
    },
    [BMS_OK] =
    {
        .port = BMS_OK_GPIO_Port,
        .pin = BMS_OK_Pin,
        .is_gpo = true,
    }
};
