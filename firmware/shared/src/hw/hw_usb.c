#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"

typedef struct
{
    USB_ModeTypeDef *const handle; // pointer to structure containing UART module configuration information
} USB;