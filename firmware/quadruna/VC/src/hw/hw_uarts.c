#include "hw_uart.h"

#include "io_chimera.h"
#include "io_sbgEllipse.h"

UartDeviceConfig uart_device_config[HW_UART_DEVICE_COUNT] = {
    [HW_UART_DEVICE_CHIMERA]     = { .uart               = &huart7,
                                     .polling_timeout_ms = osWaitForever,
                                     .callback_dma       = false,
                                     .transmit_callback  = NULL, // Doesn't use TX callbacks
                                     .receive_callback   = NULL }, // Doesn't use RX callbacks
    [HW_UART_DEVICE_SBG_ELLIPSE] = { .uart               = &huart2,
                                     .polling_timeout_ms = 0, // Doesn't use polling
                                     .callback_dma       = true,
                                     .transmit_callback  = NULL, // Doesn't use TX callbacks
                                     .receive_callback   = io_sbgEllipse_msgRxCallback },
    [HW_UART_DEVICE_MODEM_2G4]   = { .uart               = &huart3,
                                     .polling_timeout_ms = 100,
                                     .callback_dma       = false,
                                     .transmit_callback  = NULL, // Doesn't use TX callbacks
                                     .receive_callback   = NULL }, // Doesn't use RX callbacks
    [HW_UART_DEVICE_MODEM_900K]  = { .uart               = &huart1,
                                     .polling_timeout_ms = 100,
                                     .callback_dma       = false,
                                     .transmit_callback  = NULL,                    // Doesn't use TX callbacks
                                     .receive_callback   = io_chimera_msgRxCallback } // Doesn't use RX callbacks
};
