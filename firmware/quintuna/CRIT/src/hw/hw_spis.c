#include "hw_spis.h"
#include "main.h"

const SpiInterface led_spi = { .spi_handle = &hspi2,
                               .nss_port   = LED_SERIN_GPIO_Port,
                               .nss_pin    = LED_SERIN_Pin,
                               .timeout_ms = 10u };

const SpiInterface seven_seg_spi = { .spi_handle = &hspi3,
                                     .nss_port   = SEVEN_SEG_SERIN_GPIO_Port,
                                     .nss_pin    = SEVEN_SEG_SERIN_Pin,
                                     .timeout_ms = 10u };
