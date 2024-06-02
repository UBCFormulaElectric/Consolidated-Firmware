#pragma once

#include <stdbool.h>

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio test_enable_gpio;
    const Gpio n_high_current_gpio;
} BSPDTestConfig;
#else
EMPTY_STRUCT(BSPDTestConfig);
#endif

void io_bspdTest_init(const BSPDTestConfig *bspd_test_config);
void io_bspdTest_enable(bool enable);
bool io_bspdTest_isCurrentThresholdExceeded(void);