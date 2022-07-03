#pragma once

#include <stdbool.h>
#include <stm32f302xc.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t      pin;
    bool          is_gpo;
} gpio_s;

typedef enum
{
    CHARGER_EN = 0U,
    IMD_OK,
    BMS_OK,
    NUM_GPIO_USED,
} gpio_e;

void gpio_shared_write(gpio_e gpo, bool is_gpo_set);
bool gpio_shared_read(gpio_e gpo);
