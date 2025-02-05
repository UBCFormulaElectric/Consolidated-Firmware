#pragma once

#include <stdbool.h>

void io_bspdTest_enable(bool enable);
bool io_bspdTest_isCurrentThresholdExceeded(void);